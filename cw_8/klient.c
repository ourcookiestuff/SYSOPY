#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "kolejki.h"

mqd_t serwer;

void wyjscie(void)
{
	kolejka_zamknij(serwer);
}

void sygnal_wyjscie(int signal)
{
	printf("\nSygnał przerwania, zamykanie kolejki\n");
	exit(EXIT_SUCCESS);
}

unsigned int czas(void)
{
	return rand()%5; 
}

int main(int argc, char *argv[])
{
	char kolejka_odpowiedz_nazwa[10];
	mqd_t kolejka_odpowiedz;
	char odczyt[DLUGOSC_KOMUNIKATU];
	char string[DLUGOSC_KOMUNIKATU+10];
	
	sprintf(kolejka_odpowiedz_nazwa, "/%d", getpid());
	
	// chwila na uruchomieni się serwera
	sleep(1);
	
	serwer = kolejka_otworz(KOLEJKA_NAZWA, O_WRONLY);
	struct mq_attr atrybuty = kolejka_pobierz_atrybuty(serwer);
	printf("Otworzono kolejkę \"%s\" o deskryptorze %d\n mq_flags: %ld\n mq_maxmsg: %ld\n mq_msgsize: %ld\n mq_curmsgs: %ld\n\n", KOLEJKA_NAZWA, serwer, atrybuty.mq_flags, atrybuty.mq_maxmsg, atrybuty.mq_msgsize, atrybuty.mq_curmsgs);
	
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
	
	sleep(czas());
	
	while(1)
	{
		// wypisz dzialanie
		printf("Wpisz działanie\n");
		if(scanf("%s", odczyt) == EOF)
		{
			printf("\n----------------------------------------------------\n");
			printf("Koniec odczytu, można bezpiecznie zamknąć konsolę\n");
			exit(EXIT_SUCCESS);
		}
		
		// dopisz PID do komunikatu
		sprintf(string, "%d %s", getpid(), odczyt);
		
		// wyślij zapytanie na serwer
		kolejka_wyslij(serwer, string, 1);
		printf("\nWysłano zapytanie do serwera: %s\n", string);
		
		// utwórz kolejkę do odebrania odpowiedzi
		kolejka_odpowiedz = kolejka_utworz(kolejka_odpowiedz_nazwa, O_RDONLY);
		
		// czekaj na odpowiedz z serwera
		printf("Oczekiwanie na odpowiedź z serwera\n");
		kolejka_odbierz(kolejka_odpowiedz, string, NULL);
		printf("Odpowiedź z serwera: %s\n\n", string);
		
		kolejka_zamknij(kolejka_odpowiedz);
		kolejka_usun(kolejka_odpowiedz_nazwa);
		
		sleep(czas());
	}
	
	kolejka_zamknij(serwer);
	
	printf("\nKlient: koniec procesu\n");
	
	return 0;
}
