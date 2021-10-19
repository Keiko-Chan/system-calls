#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/syscall.h>

struct linux_dirent64 {

	long           d_ino;	
	off_t          d_off; 
    	unsigned short d_reclen; 
    	unsigned char  d_type;  
    	char           d_name[]; 
};

const char* type(unsigned char dtype)
{
	switch (dtype)
	{
		case DT_BLK:		return "block dev";
		case DT_CHR:		return "char dev";
		case DT_DIR:		return "directory";
		case DT_FIFO:		return "FIFO";
		case DT_LNK:		return "symlink";
		case DT_REG:		return "regular";
		case DT_SOCK:		return "socket";
		case DT_UNKNOWN:	return "???";	
	}
	return "???";
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
	if(fdd < 0)
	{
		perror("dirfd");
		return 1;
	}	
	
	int BUF_SIZE = 1024;
	int nread;
	struct linux_dirent64 *d;	
	char buf[BUF_SIZE];
	int bpos;
		
	for ( ; ; ) 
	{
               nread = syscall(SYS_getdents64, fdd, buf, BUF_SIZE);
               if (nread == -1)
                   perror("getdents64");

               if (nread == 0)
                   break;

               printf("file type     name\n");
               for (bpos = 0; bpos < nread;)
               {
                   	d = (struct linux_dirent64 *) (buf + bpos);
                   	const char *typ = type(d->d_type);
                   	printf("%-13s ", typ);
                   	printf("%s\n", d->d_name);
                   	bpos += d->d_reclen;
               }
	}  
	
	closedir(dir_fd);
	close(fdd);
	
	return 0;
}
