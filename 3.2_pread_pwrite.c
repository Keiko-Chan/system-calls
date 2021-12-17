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

int main(int argc, char *argv[])
{
	const size_t block_size = 1024 * 1024;
	const char* copy_name = "3.1_copy.txt";
	int fdo, er, fdc;
	char buffer[block_size];
	struct stat sb;
	off_t pos_r = 0;
	off_t pos_w = 0;

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
		perror("Failed to open file");
		return 2;
	}
	
	fdc = open(copy_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);		//создаю файл куда копирую
	if(fdc < 0)
	{
		perror("Failed to create file");
		return 2;
	}

	pos_r = 0;
	
	
	er = pread(fdo, buffer, block_size, pos_r);				//читаю файл
	
	while ( er != 0 )							//пока не дойду до его конца
	{
		if(er == 1)							//может случиться ошибка доступа или вроде того
		{
			perror("Error of preading");
			return 2;
		}
			
		pos_r = pos_r + block_size;
		
		
		if(pwrite(fdc, buffer, er, pos_w) < 0) 		//пишем то что прочитали в другой файл
		{
			perror("Error of pwriting");
			return 2;
		}
		
		er = pread(fdo, buffer, block_size, pos_r);			//снова читаем	
		
		pos_w = lseek(fdc, 0, 2);					//тащу указатель для записи в конец
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
