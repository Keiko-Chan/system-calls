#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/sysmacros.h>

const ssize_t ctime_r_size = 26;

void print_access(const struct stat *sb)
{
	printf("access:                   ");
	putchar( (S_ISDIR(sb->st_mode)) ? 'd' : '-');
	putchar( (sb->st_mode & S_IRUSR) ? 'r' : '-');
    	putchar( (sb->st_mode & S_IWUSR) ? 'w' : '-');
   	putchar( (sb->st_mode & S_IXUSR) ? 'x' : '-');
   	putchar( (sb->st_mode & S_IRGRP) ? 'r' : '-');
   	putchar( (sb->st_mode & S_IWGRP) ? 'w' : '-');
  	putchar( (sb->st_mode & S_IXGRP) ? 'x' : '-');
  	putchar( (sb->st_mode & S_IROTH) ? 'r' : '-');
	putchar( (sb->st_mode & S_IWOTH) ? 'w' : '-');
    	putchar( (sb->st_mode & S_IXOTH) ? 'x' : '-');
    	printf("\n");
}

char* get_file_type(const mode_t sb)
{
	switch (sb & S_IFMT)
        {
         	case S_IFBLK:  return "block device";            break;
         	case S_IFCHR:  return "character device";        break;
         	case S_IFDIR:  return "directory";               break;
         	case S_IFIFO:  return "FIFO/pipe";               break;
         	case S_IFLNK:  return "symlink";                 break;
         	case S_IFREG:  return "regular file";            break;
         	case S_IFSOCK: return "socket";                  break;
         	default:       return "unknown?";                break;
        }
        
}

int main()
{
	FILE *fp;
	struct stat sb;
	char s[ctime_r_size];
	char *type;
	struct tm bdt;
	long int nsec;
	
	if ((fp=fopen("test.txt", "rb")) == NULL) 
		perror("Failed to open file");		

	fstat (fileno (fp), &sb);
	
	type = get_file_type(sb.st_mode);
	
	printf("ID of containing device:  [%lx,%lx]\n", (long) major(sb.st_dev), (long) minor(sb.st_dev));

        printf("File type:                %s\n", type); 
        printf("I-node number:            %ld\n", (long) sb.st_ino);
        print_access(&sb);
        printf("Mode:                     %lo (octal)\n", (unsigned long) sb.st_mode);
        printf("Link count:               %ld\n", (long) sb.st_nlink);
        printf("Ownership:                UID=%ld   GID=%ld\n", (long) sb.st_uid, (long) sb.st_gid);

        printf("Preferred I/O block size: %ld bytes\n", (long) sb.st_blksize);
        printf("File size:                %lld bytes\n", (long long) sb.st_size);
        printf("Blocks allocated:         %lld\n", (long long) sb.st_blocks);
        
        ctime_r(&sb.st_ctime, s);
	nsec = sb.st_ctim.tv_nsec % 1000000000;
	localtime_r (&sb.st_ctime, &bdt);
        printf("Last status change:       %s nsec: %ld %s", bdt.tm_zone, nsec, s);
        
        ctime_r(&sb.st_atime, s);
        nsec = sb.st_atim.tv_nsec % 1000000000;
        localtime_r (&sb.st_atime, &bdt);
        printf("Last file access:         %s nsec: %ld %s", bdt.tm_zone, nsec, s);
        
        ctime_r(&sb.st_mtime, s);
        nsec = sb.st_mtim.tv_nsec % 1000000000;
        localtime_r (&sb.st_mtime, &bdt);
        printf("Last file modification:   %s nsec: %ld %s", bdt.tm_zone, nsec, s);
        
        exit(EXIT_SUCCESS);

	fclose (fp);
	
	return 0;
}
