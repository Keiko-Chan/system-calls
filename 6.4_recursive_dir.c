#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int print_dir(const char* dir_name, const int level)
{	
	DIR *dir_fd = opendir(dir_name);
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
	
	struct dirent *entry;
	
	while((entry = readdir(dir_fd)) != NULL)
	{	
		char entry_type = dtype_char(entry->d_type);
		
		if(entry_type == '?')
		{
			struct stat sb;
			if(fstatat(fdd, dir_name, &sb, 0) == 0)
			{
				entry_type = mode_char(sb.st_mode);
			}
		}
		
		for(int i = 0; i < level * 4; i++)
			putchar(' ');
			
		printf("%c %s \n", entry_type, entry->d_name);		
		
		char* name = (char*)(entry->d_name);
		
		if(entry_type == 'd' && name[0] != '.')
		{			
			char *path = (char*)calloc(strlen(name) + 1 + strlen(dir_name), sizeof(char));
			
			strcat(path, dir_name);
			
			if(path[strlen(dir_name) - 1] != '/')
				strcat(path, "/");
				
			strcat(path, name);
			strcat(path, "/");			
			
			print_dir(path, level + 1);
			
			free(path);
		}
	}
	
	
	close(fdd);
	closedir(dir_fd);
	
	return 0;
}

int main(int argc, char *argv[])
{	
	const char *dir_name = ".";
	
	if(argc == 2)
	{
		dir_name = argv[1];
	}
	
	
	int rec = print_dir(dir_name, 0);
	
	if(rec != 0)
	{
		printf("Error in print_dir");
	}	
	
	return 0;
}
