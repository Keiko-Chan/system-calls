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
	int pipe_fds[2];
	if(pipe(pipe_fds) < 0)
	{
		perror("failed to create pipe");
		return1;
	}

	pid_t child_id = fork();
	if (child_id < 0)
	{
		perror("fork");
		return 1;
	}
	
	/* this code is executed in child proccess only*/
	if (child_id == 0)
	{	
		//close resding end of th pipe
		close(pipe_fds[0]);
		//redirect stdout to pipe
		dup2(pipe_fds[1], fileno(stdout));
		close(pipe_fds[1]);
		
		execlp("last", "last", NULL);
		perror("failed to exec 'last'");
		return 0;
	}
	
	//close writing end of the pipe
	close(pipe_fds[1]);
	//redirect stdin from pipe
	dup2(pipe_fds[0], fileno(stdin));
	close(pipe_fds[0]);
	
	execlp("wc", "wc", "-l", NULL);
	perror("failed to exec 'wc -l'");

	return 0;
}
