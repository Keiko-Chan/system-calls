#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
///////////////////////////////////////
	execlp(
		// path to executabll
		"grep",
		// argv
		"grep42", "model name", "/proc/cpuinfo", NULL
	);
///////////////////////////////////////	
	//write(fileno
	printf("test 2\n");
	
	execlp(
		// path to executabll
		"echo",
		// argv
		"echo", "test 1", NULL
	);
///////////////////////////////////////	
	printf("a");
	if(fork() == 0)
	{
		printf("b");
		return 0;
	}
	printf("c");
///////////////////////////////////////	
	perror("failed to exec");
	return 0;
}
