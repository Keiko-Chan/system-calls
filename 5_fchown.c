#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<sys/sysmacros.h>
#include<string.h>
#include<stdint.h>
#include<unistd.h>

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

int main(int argc, char *argv[])
{
	const size_t block_size = 1024 * 1024;
	int fdo, er, fdc;
	const char* copy_name = "5_copy.txt";
	char buffer[block_size];
	struct stat sb;
	struct timespec for_futime[2];

	if (argc < 2)								//проверяю путь
	{
		fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
		exit(EXIT_FAILURE); 
	}
	
	if(argc > 2)
		copy_name = argv[2];
	
	if(lstat(argv[1], &sb) == -1)						//lastat to check file
	{
		perror("lstat");
		exit(EXIT_FAILURE);
	}
	
	if(S_ISREG( sb.st_mode ) == 0)
	{
		fprintf(stderr, "I can`t copy such kind of files\n");
		return 2;
	}
		
	fdo = open(argv[1], O_RDONLY, 0644);					//open file I need to copy
	if(fdo < 0)
	{
		perror("Failed to open file");
		return 2;
	}
	
	fdc = open(copy_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);		//create file for copy
	if(fdc < 0)
	{
		perror("Failed to create file");
		return 2;
	}
	
	er = read(fdo, buffer, block_size);					//read file
	
	while ( er != 0)							//while is`nt end
	{
		if(er == 1)							//reading error
		{
			perror("Error of reading");
			return 2;
		}
		
		if(writeall(fdc, buffer, er) < 0) 				//write to copy file
		{
			perror("Error of writing");
			return 2;
		}
		
		er = read(fdo, buffer, block_size);				//again read
	}
	
	if(fchmod(fdc , sb.st_mode) < 0 )					//copy access 
	{
		perror("Failure fcnmod file");
		return 3;
	}
	
	for_futime[0] = sb.st_atim;
	for_futime[1] = sb.st_mtim;	
		
	if(futimens(fdc, for_futime) < 0)					//time cope
	{
		perror("Failure futimens file");
		return 3;
	}
	
	if(fchown(fdc , sb.st_uid, sb.st_gid) < 0 )				//copy owner and group id 
	{
		perror("Failure fchown file");
		return 3;
	}
	
	
	if(close(fdo) < 0) 							
	{									//close first file
		perror("Failure white closing fd-open");
		return 4;
	}
	
	if(close(fdc) < 0) 
	{									//close second file
		perror("Failure white closing fd-create");
		return 4;
	}
	
	return 0;
}
