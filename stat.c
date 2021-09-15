#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/sysmacros.h>

char* get_file_type(const struct stat *sb)
{
	char* type;
	switch (sb->st_mode & S_IFMT)
        {
         	case S_IFBLK:  type = "block device";            break;
         	case S_IFCHR:  type = "character device";        break;
         	case S_IFDIR:  type = "directory";               break;
         	case S_IFIFO:  type = "FIFO/pipe";               break;
         	case S_IFLNK:  type = "symlink";                 break;
         	case S_IFREG:  type = "regular file";            break;
         	case S_IFSOCK: type = "socket";                  break;
         	default:       type = "unknown?";                break;
        }
        
        return type;
}

void print_access(const struct stat *sb)
{
	printf("access:                   ");
	printf( (S_ISDIR(sb->st_mode)) ? "d" : "-");
	printf( (sb->st_mode & S_IRUSR) ? "r" : "-");
    	printf( (sb->st_mode & S_IWUSR) ? "w" : "-");
   	printf( (sb->st_mode & S_IXUSR) ? "x" : "-");
   	printf( (sb->st_mode & S_IRGRP) ? "r" : "-");
   	printf( (sb->st_mode & S_IWGRP) ? "w" : "-");
  	printf( (sb->st_mode & S_IXGRP) ? "x" : "-");
  	printf( (sb->st_mode & S_IROTH) ? "r" : "-");
	printf( (sb->st_mode & S_IWOTH) ? "w" : "-");
    	printf( (sb->st_mode & S_IXOTH) ? "x" : "-");
    	printf("\n");

}

int main(int argc, char *argv[])
{
	struct stat sb;
	char s[26];
	char *type;
	struct tm bdt;
	
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
		exit(EXIT_FAILURE); 
	}
	
	if(lstat(argv[1], &sb) == -1)
	{
		perror("lstat");
		exit(EXIT_FAILURE);
	}
	
	type = get_file_type(&sb);
	
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
	localtime_r (&sb.st_ctime, &bdt);
        printf("Last status change:       %s %s", bdt.tm_zone, s);
        
        ctime_r(&sb.st_atime, s);
        localtime_r (&sb.st_atime, &bdt);
        printf("Last file access:         %s %s", bdt.tm_zone, s);
        
        ctime_r(&sb.st_mtime, s);
        localtime_r (&sb.st_mtime, &bdt);
        printf("Last file modification:   %s %s", bdt.tm_zone, s);
        
        exit(EXIT_SUCCESS);
}

