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
#include "semafory.h"
#include "dzielona.h"

#define NELE 10 // Rozmiar elementu bufora (jednostki towaru) w bajtach
#define NBUF 5 // Liczba elementow bufora

// Segment pamieci dzielonej
typedef struct {
    char bufor[NBUF][NELE]; // Wspolny bufor danych
    int wstaw, wyjmij; // Pozycje wstawiania i wyjmowania z bufora
} SegmentPD;

int czy_ostatni_blok(SegmentPD *PD)
{
    for(int i=0; i<NELE; i++)
    {
        if(PD->bufor[PD->odczyt][i] == '\0')
        {
            return 1;
        }
    }
    return 0;
}

// argumenty [0] nazwa_programu [1] SEMAFOR_PRODUCENT [2] SEMAFOR_KONSUMENT [3] NAZWA_PD [4] PLIK_WY
int main(int argc, char *argv[])
{
    int plik;
    int PD_desk;
    SegmentPD *PD;
    sem_t *semafor_producent;
    sem_t *semafor_konsument;
    
    if(argc != 5)
    {
        printf("Błąd!!! Niepoprawna liczba argumentów\n");
        exit(EXIT_FAILURE);
    }
    
    srand(time(NULL));
    
    if((plik = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
    {
        perror("Błąd otwarcia pliku do zapisu");
        exit(EXIT_FAILURE);
    }
    
    semafor_producent = otworz_semafor(argv[1]);
    printf("Konsument: otwarto semafor producenta %s, deskryptor [%p]\n", argv[1], (void *)semafor_producent);
    semafor_konsument = otworz_semafor(argv[2]);
    printf("Konsument: otwarto semafor konsumenta %s, deskryptor [%p]\n", argv[2], (void *)semafor_konsument);
    
    PD_desk = otworz_PD(argv[3]);
    printf("Konsument: pamięć dzielona %s, deskryptor [%d]\n", argv[3], PD_desk);
    
    PD = (SegmentPD *) odwzoruj_PD(PD_desk, sizeof(SegmentPD));
    
    PD->odczyt = 0;
    
    // zapisywanie skonsumowanych produktów do pliku
    while(1)
    {
        sleep(rand()%3);
        
        // opuść semafor
        printf("Konsument: przed Sekcją Krytyczną [Semafor]: %d\n", wartosc_semafor(semafor_konsument));
        opusc_semafor(semafor_konsument);
        
        // Sekcja Krytyczna
        if(!czy_ostatni_blok(PD))
        {
            // skopiuj cały blok
            if(write(plik, PD->bufor[PD->odczyt], NELE) == -1)
            {
                perror("Błąd write do potoku");
                exit(EXIT_FAILURE);
            }
            if(write(STDOUT_FILENO, ">>> Konsumuje => (10) ", 19) == -1)
            {
                perror("Błąd write na ekran");
                exit(EXIT_FAILURE);
            }
            if(write(STDOUT_FILENO, PD->bufor[PD->zapis], NELE) == -1)
            {
                perror("Błąd write na ekran");
                exit(EXIT_FAILURE);
            }
            printf("\n>>> Indeks bufora odczyt: %d\n", PD->odczyt);
        }
        else
        {
            // skopiuj ostatni fragment
            if(write(plik, PD->bufor[PD->odczyt], strlen(PD->bufor[PD->odczyt])) == -1)
            {
                perror("Błąd write do potoku");
                exit(EXIT_FAILURE);
            }
            printf(">>> Konsumuje <= (%ld) %s\n", (PD->bufor[PD->odczyt]), PD->bufor[PD->odczyt]);
            printf("Kosument: Sygnał końca odczytu\n");
            
            // podnieś semafor producenta
            podnies_semafor(semafor_producent);
            printf("Konsument: po Sekcji Krytycznej [Semafor]: %d\n", wartosc_semafor(semafor_producent));
            
            break;
        }
        
        PD->odczyt = (PD->odczyt+1)%NBUF;
        
        // podnieś semafor producenta
        podnies_semafor(semafor_producent);
        printf("Konsument: po Sekcji Krytycznej [Semafor]: %d\n", wartosc_semafor (semafor_producent));
    }
    
    if(close(plik) == -1)
    {
        perror("Błąd zamknięcia pliku wyjścia");
        exit(EXIT_FAILURE);
    }
    
    usun_odwzorowanie_PD(PD, sizeof(SegmentPD));
    
    zamknij_PD(PD_desk);
    zamknij_semafor(semafor_konsument);
    zamknij_semafor(semafor_producent);
        
    printf("Konsument: koniec procesu\n");
    
    return 0;
}
