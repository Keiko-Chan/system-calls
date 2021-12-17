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
	const char* copy_name = "3.1_copy.txt";
	int fdo, er = 1, fdc;
	char buffer[block_size];
	struct stat sb;

	if (argc < 2)								//проверяю путь
	{
		fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
		exit(EXIT_FAILURE); 
	}
	
	if(argc > 2)
		copy_name = argv[2];
	
	if(lstat(argv[1], &sb) == -1)						//lastat чтобы проверить файл
	{
		perror("lstat");
		exit(EXIT_FAILURE);
	}
	
	if(S_ISREG( sb.st_mode ) == 0)
	{
		fprintf(stderr, "I can`t copy such kind of files\n");
		return 2;
	}
		
	fdo = open(argv[1], O_RDONLY, 0644);					//открываю файл откуда копирую
	if(fdo < 0)
	{
		perror("Failed to open file1");
		return 2;
	}
	
	fdc = open(copy_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);		//создаю файл куда копирую
	if(fdc < 0)
	{
		perror("Failed to create file2");
		return 2;
	}
	
	while ( er != 0)							//пока не дойду до его конца
	{
		er = read(fdo, buffer, block_size);				//читаем
		
		if(er == -1)							//может случиться ошибка доступа или вроде того
		{
			perror("Error of reading");
			return 2;
		}	
		
		if(writeall(fdc, buffer, er) < 0) 				//пишем то что прочитали в другой файл
		{
			perror("Error of writing");
			return 2;
		}						
	}
	
	if(close(fdo) < 0) {							//закрываем один файл
		perror("Failure white closing fd-open");
		return 4;
	}
	
	if(close(fdc) < 0) {							//закрываем второй
		perror("Failure white closing fd-create");
		return 4;
	}
	
	return 0;
}
