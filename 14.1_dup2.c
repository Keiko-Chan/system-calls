#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

void proc_info(const char* procname)
{
    printf("[%s]:\n"
           "PID %d,\n"
           "PPID %d,\n"
           "PGID %d,\n"
           "SID %d\n\n", procname, getpid(), getppid(), getpgid(0), getsid(0));
}

int main(void)
{
	pid_t child_id = fork();
	
	if(child_id < 0)
	{
		perror("fork");
		return 1;
	}
	
	if(child_id == 0)
	{
		int file_fd = open("1.txt", O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if(file_fd < 0)
		{
			perror("open");
			return 1;
		}
		if(dup2(file_fd, fileno(stdout)) < 0)
		{
			perror("dup2");
			return 1;
		}
		close(file_fd);
		
		proc_info("child");
		return 0;
	}
	
	proc_info("parent");
	return 0;
}
