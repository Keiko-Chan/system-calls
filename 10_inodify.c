#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <poll.h>
#include<sys/stat.h>

#define MAX_LEN 1024 /*длина пути для директории*/

int add_watches(int fd, char *root)
{
	int wd;
	char *abs_dir;
	struct dirent *entry;
	DIR *dp;

	dp = opendir(root);
	if (dp == NULL)
	{
		perror("Error opening the starting directory");
		return 1;
	}

	wd = inotify_add_watch(fd, root, IN_CREATE | IN_MODIFY | IN_DELETE | IN_ATTRIB | IN_OPEN | IN_CLOSE | IN_MOVE); 
	if (wd == -1)
	{
		printf("Couldn't add watch to %s\n",root);
		return -1;
	}
	else
	{
		printf("Watching:: %s\n",root);
	}

	abs_dir = (char *)malloc(MAX_LEN);
	while((entry = readdir(dp)))
	{ 
		if (entry->d_type == DT_DIR && entry->d_name[0] != '.')
		{		
			strcpy(abs_dir,root);
			
			if(root[strlen(root) - 1] != '/')
				strcat(abs_dir,"/");
			
			strcat(abs_dir,entry->d_name);

			wd = inotify_add_watch(fd, abs_dir, IN_CREATE | IN_MODIFY | IN_DELETE | IN_ATTRIB | IN_OPEN | IN_CLOSE | IN_MOVE); 
			if (wd == -1)
				printf("Couldn't add watch to the directory %s\n",abs_dir);
			else
				printf("Watching:: %s\n",abs_dir);
		}
	}
  
	closedir(dp);
	free(abs_dir);
	return 0;
}

unsigned int BUF_SIZE = 64;

char* get_UTC_time(char* str, const time_t* s_time, unsigned buf_size) 
{
	struct tm *tm_time;

	tm_time = gmtime(s_time);

	if(strftime (str, buf_size, "%x %X (UTC)", tm_time) > 0) 
		return str;

	return NULL;
}

void print_events(int fd, char* argv) 
{

	char time_buf[BUF_SIZE];

	char buf[4096];
	const struct inotify_event *event;
	ssize_t len = 1;
	char *ptr;
	int x = 0;

	while(len > 0) 
	{

		len = read(fd, buf, sizeof(buf));
		if (len == -1 && errno != EAGAIN) 
		{
			perror("read");
			return;
		}

		for (ptr = buf; ptr < buf + len; ptr = ptr + sizeof(struct inotify_event) + event->len) 
		{
			event = (const struct inotify_event *) ptr;
			x = 0;
			time_t cur_time = time(NULL);

			printf("%s | ", get_UTC_time(time_buf, &cur_time, BUF_SIZE));

			if (event->mask & IN_OPEN)
			{
				x = 1;
				printf("IN_OPEN:          	");
			}
			if (event->mask & IN_CLOSE_NOWRITE)
			{
				x = 1;
				printf("IN_CLOSE_NOWRITE: 	");
			}
			if (event->mask & IN_CLOSE_WRITE)
			{
				x = 1;
				printf("IN_CLOSE_WRITE:   	");
			}
			if (event->mask & IN_ACCESS)
			{
				x = 1;
				printf("IN_ACCESS:       	");
			}
			if (event->mask & IN_CREATE)
			{
				x =1;
				printf("IN_CREATE:        	");
			}
			if (event->mask & IN_DELETE)
			{
				x = 1;
				printf("IN_DELETE:        	");
			}
			if (event->mask & IN_MODIFY)
			{
				x = 1;
				printf("IN_MODIFY:        	");
			}
			if (event->mask & IN_ATTRIB)
			{
				x = 1;
				printf("IN_ATTRIB:        	");
			}

			
			if(x == 1)
			{
				if (event->len > 0)
					printf("%s", event->name);
				else
					printf("%s", argv);

				if (event->mask & IN_ISDIR)
					printf(" [dir] \n");
				else
					printf(" [reg]\n");
			}
		}
	}
}

int main(int argc, char* argv[]) 
{
	int fd, wd, pol, i = 1;
	nfds_t nfds;
	struct stat buf;

	struct pollfd fds[1];

	if (argc < 2) 
	{
		printf("give me the path");
		return 1;
	}

	printf("Press ENTER key to terminate.\n");

	fd = inotify_init1(IN_NONBLOCK);
	if (fd == -1) 
	{
		perror("inotify_init1");
		return 1;
	}
	
	if(lstat(argv[1], &buf) < 0)
	{
		perror("lstat");
		return 1;
	}
	
	if((buf.st_mode & S_IFMT) ==  S_IFDIR)
	{
	    	if(add_watches(fd, argv[1]))
	    		wd = inotify_add_watch(fd, argv[1], IN_CREATE | IN_MODIFY | IN_DELETE | IN_ATTRIB | IN_OPEN | IN_CLOSE | IN_MOVE);
    	}
    	
    	else
		wd = inotify_add_watch(fd, argv[1], IN_CREATE | IN_MODIFY | IN_DELETE | IN_ATTRIB | IN_OPEN | IN_CLOSE | IN_MOVE);
		
	if(wd == -1)
	{
		perror("whatch");
		return -1;	
	}


	nfds = 2;

	fds[0].fd = fd; 
	fds[0].events = POLLIN; 
	
	fds[1].fd = STDIN_FILENO; 
   	fds[1].events = POLLIN; 

	printf("waiting for events\n\n");
	
	while (i == 1) 
	{

		pol = poll(fds, nfds, -1);

		if (pol == -1) 
		{
			perror("poll");
			return -1;
		}

		if (pol > 0) 
		{
			if (fds[0].revents & POLLIN) 
				print_events(fd, argv[1]);
				
			if (fds[1].revents & POLLIN) 
				break;
		}
	}
	printf("watching for events stopped.\n");

	close(fd);

	return -1;
}

