#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 700

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int signal_nr;
    int status;
    //int signum;
    signal_nr = atoi(argv[1]);
    int n=2;
    int pid = getpid();
    
    setpgid(0,0);
    
    if(signal(signal_nr, SIG_IGN) == SIG_ERR)
    {
        perror("Funkcja signal napotkala problem\n");
        exit(EXIT_FAILURE);
    }
    
    for(int i=0; i<n; i++)
    {
        int childPid = fork();
        switch(childPid)
        {
            case -1:
                perror("fork error");
                exit(EXIT_FAILURE);
            case 0:
                execl("./zad3_a","zad3_a","custom",argv[1],NULL);
            default:
                setpgid(childPid, pid);
        }
    }
    
    for(int i=0; i<n; i++)
    {
        wait(&status);
        printf("Status: %d\n", status);
    }
    
    return 0;
}
