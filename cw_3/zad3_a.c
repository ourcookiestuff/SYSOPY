#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void my_sighandler(int signal_nr)
{
    //extern const char * const sys_siglist[];
    printf("Otrzymałem sygnal %s\n", strsignal(signal_nr));
}

void default_handler(int signal_nr)
{
    if(signal(signal_nr, SIG_DFL) == SIG_ERR)
    {
        perror("Funkcja signal napotkala problem\n");
        exit(EXIT_FAILURE);
    }
}

void ignoring_handler(int signal_nr)
{
    if(signal(signal_nr, SIG_IGN) == SIG_ERR)
    {
        perror("Funkcja signal napotkala problem\n");
        exit(EXIT_FAILURE);
    }
}

void customized_handler(int signal_nr)
{
    if(signal(signal_nr, my_sighandler) == SIG_ERR)
    {
        perror("Funkcja signal napotkala problem\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Niepoprawna ilczba argumentow!!!\n");
        exit(EXIT_FAILURE);
    }
    
    printf("PID procesu: %d\n", getpid());
    
    int signal_nr;
    
    if (argv[1] != NULL && argv[2] != NULL)
    {
        signal_nr = atoi(argv[2]);
    }
    
    if(strcmp(argv[1], "default") == 0)
        default_handler(signal_nr);
        
    else if(strcmp(argv[1], "ignore") == 0)
        ignoring_handler(signal_nr);
        
    else if(strcmp(argv[1], "custom") == 0)
        customized_handler(signal_nr);
        
    else
    {
        printf("Podano zla opcje!!!\n");
        exit(1);
    }
    pause();
    
    return 0;
}
