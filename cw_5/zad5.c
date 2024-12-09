#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

char potok[20];

void wyjscie(void)
{
    unlink(potok);
}

void sygnal_wyjscie(int signal)
{
    printf("Sygnał przerwania, usuwanie potoku\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    strcpy(potok,argv[5]);
    int stat_lock;
    
    if(argc != 6)
    {
        printf ("Błąd!!! Niepoprawna liczba argumentów! Wywołaj program wpisując prog_producent prog_konsument plik_we plik_wy potok\n\n");
        _exit(EXIT_FAILURE);
    }
    
    if(mkfifo(argv[5], 0644) == -1)
    {
        perror("Błąd potoku");
        _exit(EXIT_FAILURE);
    }
    
    if(atexit(wyjscie) != 0)
    {
        perror("atexit error");
        _exit(EXIT_FAILURE);
    }
    
    if(signal(SIGINT,sygnal_wyjscie) == SIG_ERR)
    {
        perror("Błąd signal");
        exit(EXIT_FAILURE);
    }
    
    srand(time(NULL));
    
    for(int i=0; i<2; i++)
    {
        switch(fork())
        {
            case -1:
                perror("fork error");
                exit(EXIT_FAILURE);
                
            case 0:
                if(i==0)
                {
                    execl("./producent", argv[1], argv[3], argv[5], NULL);
                    perror("execl error");
                    _exit(EXIT_FAILURE);
                }
                else
                {
                    execl("./konsument", argv[2], argv[4], argv[5], NULL);
                    perror("execl error");
                    _exit(EXIT_FAILURE);
                }
                break;
            
            default:
                break;
        }
    }
    
    if(wait(&stat_lock) == -1)
    {
        perror("Błąd wait");
        _exit(EXIT_FAILURE);
    }
    
    if(wait(&stat_lock) == -1)
    {
        perror("Błąd wait");
        _exit(EXIT_FAILURE);
    }
    
    return 0;
}
