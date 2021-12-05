#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sched.h>

void printlim(int what_lim, const char* what_print)
{
	int getrl;
	struct rlimit rlim;
	
	getrl = getrlimit(what_lim, &rlim);
	
	if(getrl < 0)
	{
		perror("getrlimit");
		return;
	}	
	
	printf("%s:\n  cur limit: %-10ldmax limit: %ld\n\n", what_print, rlim.rlim_cur,  rlim.rlim_max);
}

int main(void)
{
	struct sched_param param;
	
	printlim(RLIMIT_AS, "maximum size of the process's virtual memory");
	printlim(RLIMIT_CORE, "maximum size of a core file in bytes that the process may dump");
	printlim(RLIMIT_CPU, "amount of CPU time that the process can consume, in seconds");
	printlim(RLIMIT_DATA, "maximum size of the process's data segment");
	printlim(RLIMIT_FSIZE, "maximum size in bytes of files that the process may create");
	printlim(RLIMIT_LOCKS, "combined number of flock locks and  fcntl leases  that this process may establish");
	printlim(RLIMIT_MEMLOCK, "maximum  number of bytes of memory that may be locked into RAM");
	printlim(RLIMIT_MSGQUEUE, "number of bytes that  can  be  allocated  for  POSIX  message");
	printlim(RLIMIT_NOFILE, "maximum file descriptor number that can be opened by this process");
	printlim(RLIMIT_NPROC, "limit  on  the  number  of extant process");
	printlim(RLIMIT_RSS, "limit  (in  bytes) on the process's resident set");
	printlim(RLIMIT_RTTIME, "limit (in microseconds) on the amount of CPU time that a process scheduled under  a  real-time  scheduling  policy");
	printlim(RLIMIT_SIGPENDING, "limit on the number of signals that may be queued for the real user ID of the calling process");
	printlim(RLIMIT_STACK, "maximum size of the process stack, in bytes");	
	
	printf( "PID %d, PPID %d, PGID %d, SID %d\n", getpid(), getppid(), getpgid(0), getsid(0));
	
	printf("UID %d, GID %d\n\n", getuid(), getgid());
	
	if(sched_getparam(0, &param) < 0)
	{
		perror("getparam");
		return 1;
	}
	
	printf("process priority: %d\n", param.sched_priority);
	
	return 0;
}
