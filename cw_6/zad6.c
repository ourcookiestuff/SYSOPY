#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include "semafory.h"

#define SCIEZKA "./"

char nazwa_semafora[50];
bool synchronizacja;

void wyjscie(void)
{
    if(synchronizacja)
    {
        usun_semafor(nazwa_semafora);
    }
}

void sygnal_wyjscie(int signal)
{
    if(synchronizacja)
    {
        printf("Sygnał przerwania, usuwanie semafora\n");
        usun_semafor(nazwa_semafora);
    }
    _exit(EXIT_SUCCESS);
}

// argumenty [0] nazwa_programu [1] nazwa_podprogramu [2] ilosc_inkrementatorow [3] ilosc_sekcji_krytycznych [4] plik_we_wy [5] nazwa_semafora [6] synchronizacja
int main(int argc, char*argv[])
{
    if(argc != 7)
    {
        printf("Niepoprawna ilość argumentów!!!\n");
        exit(EXIT_FAILURE);
    }
    
    if(atexit(wyjscie) != 0)
    {
        perror("atexit error");
        exit(EXIT_FAILURE);
    }
    
    if(signal(SIGINT, sygnal_wyjscie) == SIG_ERR)
    {
        perror("signal error");
        exit(EXIT_FAILURE);
    }
    
    int stat_lock;
    int ilosc_inkrementatorow;
    int plik;
    
    char temp[100];
    
    ilosc_inkrementatorow = atoi(argv[2]);
    
    strcpy(nazwa_semafora, argv[5]);
    
    //printf("%s\n", nazwa_semafora);
    
    if(atoi(argv[6]) == 0)
    {
        synchronizacja = false;
    }
    else
    {
        synchronizacja = true;
    }
    
    if((plik = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
    {
        perror("Błąd otwarcia pliku do zapisu");
        exit(EXIT_FAILURE);
    }
    
    if(write(plik, "0", 2) == -1)
    {
        perror("Błąd write do pliku");
        exit(EXIT_FAILURE);
    }
    
    if(close(plik) == -1)
    {
        perror("Błąd zamknięcia pliku");
        exit(EXIT_FAILURE);
    }
    
    if(synchronizacja)
    {
        sem_t *semfor = stworz_semafor(nazwa_semafora);
        int wartosc_semafora;
        wartosc_semafor(semfor, &wartosc_semafora);
        printf("Utworzono semafor: %s [%p] o wartości %d\n", nazwa_semafora, (void *)semfor, wartosc_semafora);
    }
    
    for(int i=0; i<ilosc_inkrementatorow; i++)
    {
        switch(fork())
        {
            case -1:
                perror("fork error");
                exit(EXIT_FAILURE);
            case 0:
                sprintf(temp, "%s%s", SCIEZKA, argv[1]);
                execl(temp, argv[1], argv[3], argv[4], argv[5], argv[6], NULL);
                perror("execl error");
                exit(EXIT_FAILURE);
                
                break;
            default:
                break;
        }
    }
    
    for(int i=0; i<ilosc_inkrementatorow; i++)
    {
        if(wait(&stat_lock) == -1)
        {
            perror("Błąd wait");
            exit(EXIT_FAILURE);
        }
    }
    
    if((plik = open(argv[4], O_RDONLY, 0644)) == -1)
    {
        perror("Błąd otwarcia pliku do odczytu");
        exit(EXIT_FAILURE);
    }
    
    int odczyt;
    char bufor[10];
    
    odczyt = read(plik, &bufor, 10);
    if(odczyt == -1)
    {
        printf("Błąd odczytu z pliku\n");
        exit(EXIT_FAILURE);
    }
    else if(odczyt == 0)
    {
        printf("Plik jest pusty\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        bufor[odczyt] = '\0';
        if(atoi(bufor) == (atoi(argv[2]) * atoi(argv[3])))
        {
            printf("\nInkrementacja zakończona pomyślnie! Liczba w pliku: %s\n", bufor);
        }
        else
        {
            printf("\nInkrementacja nie powiodła się! Liczba w pliku: %s, oczekiwano: %d\n", bufor, (atoi (argv[2]) * atoi (argv[3])));
        }
    }
    
    if(close(plik) == -1)
    {
        perror("Błąd zamknięcia pliku");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
