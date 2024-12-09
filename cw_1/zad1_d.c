#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
 
int main(){
    
    printf("Parent process:\n");
    printf("UID: %d\n", getuid());
    printf("GID: %d\n", getgid());
    printf("PID: %d\n", getpid());
    printf("PPID: %d\n", getppid());
    printf("PGID: %d\n", getpgrp());
    printf("\n");
    
    for(int i=0; i<3; i++) {
        switch(fork()){
                
            case -1:
                perror("fork error");
                exit(EXIT_FAILURE);
                //break;
                
            case 0:
                printf("Pokolenie nr. %d\n", i+1);
                printf("UID: %d\n", getuid());
                printf("GID: %d\n", getgid());
                printf("PID: %d\n", getpid());
                printf("PPID: %d\n", getppid());
                printf("PGID: %d\n", getpgrp());
                printf("\n");
                
                sleep(2);
                
                //exit(EXIT_SUCCESS);
                break;
                
            default:
                sleep(2);
                break;
        }
    }
 
    return 0;
}
