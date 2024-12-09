#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include "semafory.h"

int main( int argc, char *argv[])
{
    int odczyt;
    char bufor[100];
    int liczba;
    int ilosc_sekcji_krytycznych;
    sem_t *semafor;
    int wartosc_semafora;
    int plik;
    bool synchronizacja;
    
    if(argc != 5)
    {
        printf("Błąd!!! Niepoprawna ilość argumwntów");
        _exit(EXIT_FAILURE);
    }
    
    ilosc_sekcji_krytycznych = atoi(argv[1]);
    
    if(atoi(argv[4]) == 0)
    {
        synchronizacja = false;
    }
    else
    {
        synchronizacja = true;
    }
    
    if(synchronizacja)
    {
        semafor = otworz_semafor(argv[3]);
    }
    
    for(int i=0; i<ilosc_sekcji_krytycznych; i++)
    {
        // Wlasne sprawy
        
        srand(time(NULL));
        sleep(rand() % 3 + 1);
        
        if(synchronizacja)
        {
            wartosc_semafor(semafor, &wartosc_semafora);
            printf("[PID]: %d przed Sekcją Krytyczną [Semafor]: %d\n", getpid(), wartosc_semafora);
            opusc_semafor(semafor);
        }
        
        // Sekcja krytyczna
        
        if((plik = open(argv[2], O_RDONLY, 0644)) == -1)
        {
            perror("Błąd otwarcia pliku do odczytu");
            _exit(EXIT_FAILURE);
        }
        
        odczyt = read(plik, &bufor, 10);
        if(odczyt == -1)
        {
            printf("Błąd odczytu z pliku\n");
            _exit(EXIT_FAILURE);
        }
        else if(odczyt == 0)
        {
            break;
        }
        else
        {
            bufor[odczyt] = '\0';
            wartosc_semafor(semafor, &wartosc_semafora);
            printf(">>>> [PID]: %d [Semafor]: %d [Odczytany numer]: %s [Numer Sekcji Krytycznej]: %d\n", getpid(), wartosc_semafora, bufor, i);
        }
        
        if(close(plik) == -1)
        {
            perror("Błąd zamknięcia pliku");
            exit(EXIT_FAILURE);
        }
        
        liczba = atoi(bufor);
        liczba++;
        sprintf(bufor, "%d", liczba);
        //printf("liczba %d\n", liczba);
        
        sleep(rand() % 3 + 1);
        
        if((plik = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
        {
            perror("Błąd otwarcia pliku do zapisu");
            _exit(EXIT_FAILURE);
        }
        
        if(write(plik, &bufor, strlen(bufor)) == -1)
        {
            perror("Błąd write do pliku");
            _exit(EXIT_FAILURE);
        }
        
        if(close(plik) == -1)
        {
            perror("Błąd zamknięcia pliku");
            exit(EXIT_FAILURE);
        }
        
        if(synchronizacja)
        {
            podnies_semafor(semafor);
            wartosc_semafor(semafor, &wartosc_semafora);
            printf("[PID]: %d pod Sekcji Krytycnej [Semafor]: %d\n", getpid(), wartosc_semafora);
        }
        
        // Reszta
        
    }
    
    if(synchronizacja)
    {
        zamknij_semafor(semafor);
    }
    
    return 0;
}
