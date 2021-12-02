#include <unistd.h>
#include <sys/file.h>
#include <stdio.h>
#include <errno.h>

int main(void)
{
    int counterFd = open ("counter.txt", O_CREAT | O_RDWR, 0666);
    int count;
    size_t buf;
    
    if (counterFd  < 0) 
    {
    	perror("open");
        return -1;
    }
    
    if (flock (counterFd, LOCK_EX | LOCK_NB) < 0) 
    {
    	perror("flock");
        return -1;
    }
    
    FILE* fp = fdopen(counterFd, "w+");   
    
    
    buf = fread(&count, sizeof(count), 1, fp);

    if (buf < 1) 
    {
        count = 1;
        fwrite(&count, sizeof(count), 1, fp);
    }
    
    else
    {
        count++;
        rewind(fp);
        fwrite(&count, sizeof(count), 1, fp);
    }
    
    fflush(fp);
    printf("Program has been run %d times\n", count);

    flock (counterFd, LOCK_UN);
    close(counterFd);
}
