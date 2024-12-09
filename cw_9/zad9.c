#define _REENTRANT         //kod wielokrotnego uzytku
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "watki.h"

int liczba;
unsigned int liczba_sekcji;
unsigned int liczba_watkow;
pthread_t watki[10];
pthread_mutex_t mutex;

unsigned int czas(void)
{
    return 2 + rand() % 5;
}

void gotoxy(unsigned x, unsigned y)
{
  printf("\033[%d;%dH\033[2K", y, x);
}

void *watek_start(void *argument)
{
    int licznik = 0;
    int numer_watku = *(int *)argument;
    
    srand(time(NULL));
    
    printf("Wątek nr: [%d], ID: [%ld]\n", numer_watku+1, watki[numer_watku]);
    
    sleep(3);
    
    for(int i=0; i<liczba_sekcji; i++)
    {
        gotoxy(0, 4+liczba_watkow+numer_watku);
        printf("\033[34mWątek nr: %d, Sekcja Prywatna: %d, liczba: %d \033[0m\n", numer_watku + 1, i + 1, liczba);
        
        sleep(czas());
        
        // Sekcja Krytyczna
        zamknij_mutex(&mutex);
        
        gotoxy(46, 4+liczba_watkow+numer_watku);
        printf("\033[1;36mWątek nr: %d, Sekcja Krytyczna: %d, liczba: %d \033[0m\n", numer_watku + 1, i + 1, liczba);
        
        // dodaj +1 do liczby
        licznik = liczba;
        licznik++;
        
        sleep(czas());
        
        liczba=licznik;
        
        // koniec Sekcji Krytycznej
        otworz_mutex(&mutex);
        
        gotoxy(0,4+liczba_watkow+numer_watku);
        printf("\033[34mWątek nr: %d, Sekcja Prywatna: %d, liczba: %d \033[0m\n", numer_watku + 1, i + 1, liczba);
        
        sleep(czas());
    }
    
    return 0;
}

// argumenty [0] nazwa_programu [1] ilosc_watkow [2] ilosc_sekcji
int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Błąd!!! Niepoprawna liczba argumentow!\n");
        exit(EXIT_FAILURE);
    }
    
    liczba_watkow = atoi(argv[1]);
    liczba_sekcji = atoi(argv[2]);
    
    inicjalizuj_mutex(&mutex);
    printf("Zainicjalizowano mutex o adresie %p\n", (void *)&mutex);
    
    liczba = 0;
    
    // utwórz podaną liczbę wątków
    for(int i=0; i<liczba_watkow; i++)
    {
        int *pamiec = malloc(sizeof(int));
        *pamiec = i;
        
        // utwórz wątek i przekaż mu jako argumenty jego numer
        sleep((double)i/liczba_watkow);
        watki[i] = utworz_watek(watek_start, (void *)pamiec);
    }
    
    // czekaj na zakończenie wątków
    for(int i=0; i<liczba_watkow; i++)
    {
        polacz_watek(watki[i]);
    }
    
    gotoxy(0, 4+liczba_watkow*2);
    
    // sprawdź czy synchronizacja się powiodła
    if(liczba != liczba_watkow*liczba_sekcji)
    {
        printf("Synchronizacja nie powiodła się!!!\nOczekiwano: [%d]\nOtrzymano: [%d]\n", (liczba_watkow * liczba_sekcji), liczba);
    }
    else
    {
        printf ("Synchronizacja powiodła się!\nOczekiwano: [%d]\nOtrzymano: [%d]\n", (liczba_watkow * liczba_sekcji), liczba);
    }
    
    usun_mutex(&mutex);
    
    return 0;
}
