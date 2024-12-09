#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    
    pid_t parent_pid = getpid();
    //char command[50];
    
    printf("Parent process:\n");
    printf("UID: %d\n", getuid());
    printf("GID: %d\n", getgid());
    printf("PID: %d\n", parent_pid);
    printf("PPID: %d\n", getppid());
    printf("PGID: %d\n", getpgrp());
    printf("\n");

    
    for(int i=0; i<3; i++) {
        pid_t pid = fork();
        
        if(pid==-1)
        {
            perror("fork error");
            exit(EXIT_FAILURE);
        }
        if(pid==0)
        {
            sleep(2);
            printf("Child process %d UID: %d GID: %d PID: %d PPID: %d PGID: %d\n", i+1,getuid(),getgid(),getpid(),getppid(),getpgrp());
            exit(0);
        }
    }
    sleep(1);
    /*sprintf(command, "pstree -p %d", parent_pid);
    system(command);*/
    
    exit(0);
}
