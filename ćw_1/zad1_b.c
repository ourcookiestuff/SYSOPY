#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t parent_pid = getpid();

    // Wyświetlenie identyfikatorów dla procesu macierzystego
    printf("Parent process:\n");
    printf("UID: %d\n", getuid());
    printf("GID: %d\n", getgid());
    printf("PID: %d\n", parent_pid);
    printf("PPID: %d\n", getppid());
    printf("PGID: %d\n", getpgrp());
    printf("\n");

    // Uruchomienie trzech procesów potomnych w pętli for
    for (int i = 0; i < 3; i++) {
        
        switch(fork()) {
            case -1:
                perror("fork error");
                exit(EXIT_FAILURE);
                
            case 0:
                // Wyświetlenie identyfikatorów dla procesu potomnego
                printf("Child process %d:\n", i+1);
                printf("UID: %d\n", getuid());
                printf("GID: %d\n", getgid());
                printf("PID: %d\n", getpid());
                printf("PPID: %d\n", getppid());
                printf("PGID: %d\n", getpgrp());
                printf("\n");

                // Zakończenie procesu potomnego
                exit(EXIT_SUCCESS);
            
            default:
                wait(NULL);
                break;
                
        }
    }

    return 0;
}

