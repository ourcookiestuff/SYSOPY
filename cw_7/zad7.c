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
//#include <semaphore.h>
#include "dzielona.h"
#include "semafory.h"

#define NELE 20 // Rozmiar elementu bufora (jednostki towaru) w bajtach
#define NBUF 5 // Liczba elementow bufora
#define PLIK_WE "input"
#define PLIK_WY "output"
#define SEMAFOR_PRODUCENT "/sem_producent"
#define SEMAFOR_KONSUMENT "/sem_konsument"
#define NAZWA_PD "/prod_kons_pd"

// Segment pamieci dzielonej
typedef struct {
    char bufor[NBUF][NELE]; // Wspolny bufor danych
    int wstaw, wyjmij; // Pozycje wstawiania i wyjmowania z bufora
} SegmentPD;

void sygnal_wyjscie(int signal)
{
    printf("\nSygnał przerwania, usuwanie semaforów i pamięci dzielonej\n");
    exit(EXIT_SUCCESS);
}

void wyjscie(void)
{
    usun_PD(NAZWA_PD);
    usun_semafor(SEMAFOR_PRODUCENT);
    usun_semafor(SEMAFOR_KONSUMENT);
}

int main(int argc, char *argv[])
{
    int PD_desk;
    int stat_lock;
    sem_t *semafor;
    
    if(argc != 3)
    {
        printf("Błąd!!! Niepoprawna liczba argumentów\n");
        exit(EXIT_FAILURE);
    }
    
    semafor = stworz_semafor(SEMAFOR_PRODUCENT, NBUF);
    printf("Utworzono semafor producenta %s (%p) o wartości początkowej: %d\n", SEMAFOR_PRODUCENT, (void *)semafor, wartosc_semafor(semafor));
    semafor = stworz_semafor(SEMAFOR_KONSUMENT, 0);
    printf("Utworzono semafor producenta %s (%p) o wartości początkowej %d\n", SEMAFOR_KONSUMENT, (void *)semafor, wartosc_semafor(semafor));
    
    PD_desk = stworz_PD(NAZWA_PD);
    ustaw_rozmiar_PD(PD_desk, sizeof(SegmentPD));
    printf("Utworzono pamięć dzieloną %s o deskryptorze %d i rozmiarze %ld\n\n\n", NAZWA_PD, PD_desk, sizeof(SegmentPD));
    
    if(atexit(wyjscie) != 0)
    {
        perror("atexit error");
        _exit(EXIT_FAILURE);
    }
    
    if(signal(SIGINT, sygnal_wyjscie) == SIG_ERR)
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
                    execl("./producent", argv[1], SEMAFOR_PRODUCENT, SEMAFOR_KONSUMENT, NAZWA_PD, PLIK_WE, NULL);
                    perror("Błąd exexl");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    execl("./konsument", argv[2], SEMAFOR_PRODUCENT, SEMAFOR_KONSUMENT, NAZWA_PD, PLIK_WY, NULL);
                    perror("Błąd exexl");
                    exit(EXIT_FAILURE);
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
    
    zamknij_PD(PD_desk);
    
    printf("\n\nProgram główny: koniec procesu\n");
    
    return 0;
}
