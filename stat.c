#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/sysmacros.h>
//#include<iostream>

int main(int argc, char *argv[])
{
	struct stat sb;
	char s[26];
	
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
	
	printf("ID of containing device:  [%lx,%lx]\n", (long) major(sb.st_dev), (long) minor(sb.st_dev));

        printf("File type:                ");

        switch (sb.st_mode & S_IFMT)
        {
         	case S_IFBLK:  printf("block device\n");            break;
         	case S_IFCHR:  printf("character device\n");        break;
         	case S_IFDIR:  printf("directory\n");               break;
         	case S_IFIFO:  printf("FIFO/pipe\n");               break;
         	case S_IFLNK:  printf("symlink\n");                 break;
         	case S_IFREG:  printf("regular file\n");            break;
         	case S_IFSOCK: printf("socket\n");                  break;
         	default:       printf("unknown?\n");                break;
        }

        printf("I-node number:            %ld\n", (long) sb.st_ino);
        printf("Mode:                     %lo (octal)\n", (unsigned long) sb.st_mode);
        printf("Link count:               %ld\n", (long) sb.st_nlink);
        printf("Ownership:                UID=%ld   GID=%ld\n", (long) sb.st_uid, (long) sb.st_gid);

        printf("Preferred I/O block size: %ld bytes\n", (long) sb.st_blksize);
        printf("File size:                %lld bytes\n", (long long) sb.st_size);
        printf("Blocks allocated:         %lld\n", (long long) sb.st_blocks);

	ctime_r(&sb.st_ctime, s);
        printf("\nLast status change:       %s", s);
        ctime_r(&sb.st_atime, s);
        printf("Last file access:         %s", s);
        ctime_r(&sb.st_mtime, s);
        printf("Last file modification:   %s", s);

        exit(EXIT_SUCCESS);
}

