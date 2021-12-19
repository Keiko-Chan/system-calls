#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

// получаем информацию о процессе
void proc_info(const char* procname)
{
    printf("[%s]:\n"
           "PID %d,\n"
           "PPID %d,\n"
           "PGID %d,\n"
           "SID %d\n\n", procname, getpid(), getppid(), getpgid(0), getsid(0));
}

int main(int argc, char *argv[])
{
	pid_t child, w;
	int status;

	child = fork();	
    	
	if (child == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (child == 0)
	{            
		proc_info("Child");
		if (argc == 1)
    			pause();            
		_exit(atoi(argv[1]));   
	}
	
	else 
	{                      

        	proc_info("Parent");
        	while(1)
        	{
            		w = waitpid(child, &status, WUNTRACED | WCONTINUED);	//WUNTRACED - возвращаться, если есть остановленный потомок  WCONTINUED - также возвращаться, если работа остановленного потомка возобновилась из-за получения сигнала SIGCONT. 
            
            		if (w == -1)
            		{
                		perror("waitpid");
                		exit(EXIT_FAILURE);
            		}

			if (WIFEXITED(status))	
			//возвращает истинное значение, если потомок нормально завершился, то есть вызвал exit(3) или _exit(2), или вернулся из функции main(). 
			{
				printf("exited, status=%d\n", WEXITSTATUS(status));
			}
			else 
				if (WIFSIGNALED(status))
				//возвращает истинное значение, если потомок завершился из-за сигнала. 
				{
					printf("killed by signal %d\n", WTERMSIG(status));
				}
				else 
					if (WIFSTOPPED(status))
					//возвращает истинное значение, если потомок остановлен по сигналу; нужен WUNTRACED
					{
						printf("stopped by signal %d\n", WSTOPSIG(status));
					}
					else 
						if (WIFCONTINUED(status))
						//возвращает истинное значение, если потомокпродолжил работу, получив сигнал SIGCONT. 
						{
							printf("continued\n");
						}
            		         
			if(WIFEXITED(status) || WIFSIGNALED(status))    
				break;        			

        	}

		#ifdef WCOREDUMP
		if (WCOREDUMP(status)) 
		{
    			printf("child proc has cause a core dump\n");
		}
		#endif 

		exit(EXIT_SUCCESS);
    	}
}
