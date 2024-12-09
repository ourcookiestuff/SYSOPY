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
    if (argc != 3)
    {
        printf("Niewlasciwa liczba argumentow.\n");
        exit(EXIT_FAILURE);
    }
    
    int childPid;
    int signal_nr;
    int status;
    //int signum;
    
    if (argv[1] != NULL && argv[2] != NULL)
    {
        signal_nr = atoi(argv[2]);
    }
    
    printf("PID procesu macierzytego: %d\n", getpid());
    switch(childPid = fork())
    {
        case -1:
            perror("fork error");
            exit(EXIT_FAILURE);
        case 0:
            sleep(1);
            execl("./zad3_a","zad3_a",argv[1],argv[2],NULL);
            perror("exec error");
            exit(EXIT_FAILURE);
        default:
            sleep(2);
            kill(childPid,0);
            if(errno==ESRCH)
            {
                printf("Proces nie istnieje\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                kill(childPid, signal_nr);
            }
            wait(&status);
            if (WIFEXITED(status))
            {
                printf("Proces potomny zakończył się normalnie, status: %d\n", WEXITSTATUS(status));
            }
            else if (WIFSIGNALED(status))
            {
                //signum = WTERMSIG(status);
                //extern const char * const sys_siglist[];
                printf("Proces potomny został zakończony sygnałem %s\n", strsignal(signal_nr));
            }
    }
    
    return 0;
}
