#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <sys/sysmacros.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include<stdint.h>


char mode_char(unsigned mode)
{
	switch (mode & S_IFMT)
	{
         	case S_IFBLK:  return 'b';	break;
         	case S_IFCHR:  return 'c';	break;
         	case S_IFDIR:  return 'd';	break;
         	case S_IFIFO:  return 'f';	break;
         	case S_IFLNK:  return 'l';	break;
         	case S_IFREG:  return 'r';	break;
         	case S_IFSOCK: return 's';	break;
         	default:       return '?';	break;
        }
        return '?';
}

char dtype_char(unsigned dtype)
{
	switch (dtype)
	{
		case DT_BLK:		return 'b';
		case DT_CHR:		return 'c';
		case DT_DIR:		return 'd';
		case DT_FIFO:		return 'f';
		case DT_LNK:		return 'l';
		case DT_REG:		return 'r';
		case DT_SOCK:		return 's';
		case DT_UNKNOWN:	return '?';	
	}
	return '?';
}

ssize_t writeall(int fd, const void *buf, size_t count)
{
	size_t bytes_written = 0;
	const uint8_t *buf_addr = buf;
	while (bytes_written < count) 
	{
		ssize_t res = write(fd, buf_addr + bytes_written, count - bytes_written);
		if(res < 0) 
		{
			return res;
			
		}
		bytes_written += (size_t)res;
	}
	return (ssize_t)bytes_written;
}

int copy_reg(const char* what, const char* where,  const char* how_to_name)
{
	const size_t block_size = 1024;
	int fdo, er = 1, fdc;
	char buffer[block_size];

		
	fdo = open(what, O_RDONLY, 0644);					
	if(fdo < 0)
	{
		perror("Failed to open file for copy");
		return 1;
	}
	
	char* new_file = (char*)calloc(strlen(where) + strlen(how_to_name), sizeof(char));
	strcat(new_file, where);
	strcat(new_file, how_to_name);
	
	fdc = creat(new_file, S_IREAD | S_IWRITE);				
	if(fdc < 0)
	{
		perror("Failed to create file");
		return 1;
	}
	
	while ( er != 0)							
	{
		er = read(fdo, buffer, block_size);				
		
		if(er == -1)							
		{
			perror("Error of reading");
			return 2;
		}	
		
		if(writeall(fdc, buffer, er) < 0) 				
		{
			perror("Error of writing");
			return 2;
		}						
	}
	
	if(close(fdo) < 0) {							
		perror("Failure white closing fd-open");
		return 3;
	}
	
	if(close(fdc) < 0) {							
		perror("Failure white closing fd-create");
		return 3;
	}
	
	free(new_file);

	return 0;
}

int copy_dir(const char* what, const char* where, const char* how_to_name, mode_t mode)
{
	DIR *dir_fd = opendir(what);
	if (dir_fd == NULL) 
	{	
		perror("opendir");
		return 1;
	}
	
	int fdd = dirfd(dir_fd); 				
	if(fdd < 0)
	{
		perror("dirfd");
		return 1;
	}
	
	int where_dir = open(where, O_RDONLY, O_DIRECTORY);
    	if (!where_dir) 
    	{
        perror("Can't open 'where' directory");
        return 1;
    	}
	
	if (mkdirat(where_dir, how_to_name, mode) < 0) 
	{
        	perror("Can`t create directory");
        	return 1;
    	}	
	
	struct dirent *entry;
	
	while((entry = readdir(dir_fd)) != NULL)
	{	
		char entry_type = dtype_char(entry->d_type);
		struct stat sb;
		
		int fst = fstatat(fdd, (char*)entry->d_name, &sb, 0);
		if(fst != 0)
		{
			perror("fstatat");
        		return 4;
		}
	
		
		if(entry_type == '?')
		{
			entry_type = mode_char(sb.st_mode);
		}
		
		char* name = (char*)(entry->d_name);
		
		if(entry_type == 'r')
		{			
			char *copyto = (char*)calloc(strlen(name) + strlen(what) + 1, sizeof(char));
			
			char *path = (char*)calloc(strlen(how_to_name) + strlen(where) + 1, sizeof(char));
			
			strcat(path, where);			
			strcat(path, how_to_name);
			strcat(path, "/");
			
			strcat(copyto, what);
			strcat(copyto, name);	
												
			copy_reg(copyto, path, name);
			
			free(copyto);
			free(path);
		}
		
		if((entry_type == 'd') && !((name[0] == 'c' && name[1] == 'o' && name[2] == 'p' && name[3] == 'y' && name[4] == '_') || name[0] == '.'))
		{				
			char *copyto = (char*)calloc(strlen(name) + 1 + strlen(what), sizeof(char));
			
			char *path = (char*)calloc(strlen(how_to_name) + strlen(where) + 1, sizeof(char));
			
			strcat(path, where);	
			strcat(path, how_to_name);
			strcat(path, "/");
				
			strcat(copyto, what);	
			strcat(copyto, name);
			strcat(copyto, "/");			
									
			copy_dir(copyto, path, name, sb.st_mode);
			
			free(copyto);
			free(path);
		}
	}
	
	
	closedir(dir_fd);
	close(fdd);
	close(where_dir);
	
	
	return 0;
}


int main(int argc, char *argv[])
{	
	const char *name = "./";
	char *dir_name;
	
	if(argc == 2)
	{
		name = argv[1];
	}
	
	int i = strlen(name);
	size_t len = 0;
	
	
	if(name[i - 1] != '/')
	{
		dir_name = (char*)calloc(strlen(name) + 1, sizeof(char));
		strcat(dir_name, name);
		strcat(dir_name, "/"); 	
	}
	
	else
	{
		dir_name = (char*)calloc(strlen(name), sizeof(char));
		strcat(dir_name, name);
	}

	
	i = strlen(dir_name) - 2;	
	
	while(i >= 0 && dir_name[i] != '/' )
	{
		len++;
		i--;
	}
	
	char* copy_name = (char*)calloc(len + 5, sizeof(char));
	i++;
	len = 0;
	
	while(dir_name[i] != '/')
	{
		copy_name[len + 5] = dir_name[i];
		i++;
		len++;
	}
	
	copy_name[4] = '_';
	copy_name[0] = 'c';
	copy_name[1] = 'o';
	copy_name[2] = 'p';
	copy_name[3] = 'y';
	
	int fdd = open(dir_name, O_RDONLY, 0644); 				
	if(fdd < 0)
	{
		perror("open");
		return 1;
	}
	
	struct stat buf;
	
	int lst = lstat(dir_name, &buf);
	if(lst != 0)
	{
		perror("lstat");
		return 4;
	}
	
	if(mode_char(buf.st_mode) != 'd')
	{
		printf("need directory to copy");
		return 1;
	}
	
	close(fdd);
	
	int res = copy_dir(dir_name, dir_name, copy_name, buf.st_mode);
	if(res != 0)
	{
		printf("copy dir error");
		return 1;
	}
	
	free(copy_name);
	return 0;
}
