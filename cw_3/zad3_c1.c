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
    int pgid;
    int status;
    
    if(argv[1] != NULL)
    {
        signal_nr = atoi(argv[1]);
    }
    
    if(argv[1] == NULL)
    {
        printf("Nie podano nr sygnalu!!!\n");
        exit(2);
    }
    
    printf("PID procesu macierzystego: %d\n", getpid());
    int childPid = fork();
    switch(childPid)
    {
        case -1:
            perror("fork error");
            exit(EXIT_FAILURE);
        case 0:
            sleep(1);
            execl("./zad3_c2", "zad3_c2", argv[1], NULL);
            perror("exec error");
            exit(EXIT_FAILURE);
        default:
            sleep(4);
            pgid = getpgid(childPid);
            kill(-pgid,0);
            if(errno == ESRCH)
            {
                printf("Proces nie istnieje!!!\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                kill(-pgid, signal_nr);
            }
            wait(&status);
            printf("PID bezpośredniego potomka: %d\n", childPid);
            printf("Status tego u gory: %d\n", status);
    }
    return 0;
}
