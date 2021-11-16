#include <sys/stat.h>
#include <stdio.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <unistd.h>


void PrintDirInfo(const char* dir_name, struct statvfs* info)
{
	printf("Name: %s\n\n", dir_name);

	printf("Block size:            			%lu\n", info->f_bsize);
	printf("Fragment size:        			%lu\n", info->f_frsize);
	printf("Size in blocks:        			%lu\n", info->f_blocks);
	printf("Number of free blocks: 			%lu\n", info->f_bfree);
	printf("Number of free blocks for unprivileged users: %lu\n", info->f_bavail);
	printf("Number of inodes:                             %lu\n", info->f_files);
	printf("Number of free inodes:                        %lu\n", info->f_ffree);
	printf("Number of free inodes for unprivileged users: %lu\n", info->f_favail);
	printf("Filesystem ID:                                %lu\n", info->f_fsid);
	printf("Moubt flags:		                       %lu\n", info->f_flag);
	printf("Maximum filename length:                      %lu\n", info->f_namemax);

}

int main(int argc, char* argv[])
{
	const char* dir_name = "./";
	char* file_name;
	struct stat buf1, buf2;
	struct statvfs info;
	long have_place;
	
	if(argc < 2)
	{
		printf("Give me file and directory");
		return 1;
	}
	
	if(argc == 2)
	{
		file_name = argv[1];
	}
	
	if(argc == 3)
	{
		file_name = argv[1];
		dir_name = argv[2];
	}
	
	if(lstat(dir_name, &buf1) < 0)
	{
		perror("lstat1");
		return 2;
	}
	
	if(lstat(file_name, &buf2) < 0)
	{
		perror("lstat2");
		return 2;
	}
	
	if((buf1.st_mode & S_IFMT) != S_IFDIR)
	{
		printf("%s - is not a directory\n", dir_name);
		return 1;
	}
	
	if(statvfs(dir_name, &info) < 0)
	{
		perror("statvfs");
		return 2;
	}
	
	have_place = (long)(info.f_bavail * info.f_bsize);
	
	PrintDirInfo(dir_name, &info);
	
	if(have_place >= buf2.st_size && info.f_favail >= 1)
		printf("\n File %s can fit in the %s", file_name, dir_name);
		
	else
		printf("\n File %s can`t fit in the %s", file_name, dir_name);
	
  	
  	return 0;
}
