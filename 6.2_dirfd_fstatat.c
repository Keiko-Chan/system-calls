#include<stdio.h>
#include<dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<stdio.h>

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

int main(int argc, char *argv[])
{	
	const char *dir_name = ".";
	
	if(argc == 2)
	{
		dir_name = argv[1];
	}
	
	DIR *dir_fd = opendir(dir_name);
	if (dir_fd == NULL) 
	{	
		perror("opendir");
		return 1;
	}
	
	int fdd = dirfd(dir_fd); 				
	
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
		
		printf("%c %s \n", entry_type, entry->d_name);
	}
	
	
	closedir(dir_fd);
	close(fdd);
	
	return 0;
}
