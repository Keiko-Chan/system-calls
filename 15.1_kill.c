#include <signal.h>
#include <unistd.h>
#include <stdio.h>

volatile int g_last_signal;	//volatile sigatomic_t g_last_signal

void sig_handler(int signum)
{
	g_last_signal = signum;
}

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
	proc_info("Current process:");

	if(signal(SIGINT, sig_handler) == SIG_ERR)		//Прерывание с клавиатуры
	{
		perror("SIGINT signal");
		return 1;
	}
	
	if(signal(SIGQUIT, sig_handler) == SIG_ERR)		//Выход с клавиатуры
	{
		perror("SIGQUIT signal");
		return 1;	
	}
	
	if(signal(SIGTSTP, sig_handler) == SIG_ERR)		//Остановка, введенная на терминале
	{
		perror("SIGTSTP signal");
		return 1;
	}
	
	if(signal(SIGHUP, sig_handler) == SIG_ERR)		//зависание на управляющем терминале или завершение процесса управления
	{
		perror("SIGHUP signal");
		return 1;
	}
	
	if(signal(SIGTERM, sig_handler) == SIG_ERR)		//Сигнал завершения
	{
		perror("SIGTERM signal");
		return 1;
	}
	
	//signal(SIGKILL, sig_handler);			//так нельзя

	while(1)
	{
		pause(); 
		printf("Signal %d came\n", g_last_signal);
	}
	return 0;
}
