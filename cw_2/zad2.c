#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

int main()
{
    switch( fork() ){
        case -1:
            perror("Blad funkcji fork");
            _exit(EXIT_FAILURE);
            //break;
        case 0:
            execl("./zad1_b","zad1_b.x",(char *) NULL);
            perror("blad execl");
            exit(EXIT_SUCCESS);
 
            break;
        default:
            wait(0);
            break;
    }
 
    return 0;
}
