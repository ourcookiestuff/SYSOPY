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

int main(int argc, char* argv[])
{
    int text_file_descriptor;
    int potok;
    char bufor[1024];
    const size_t rozmiarDanychKonsument = 7;
    int odczyt;
    
    if(argv[1] == NULL || argv[2] == NULL)
    {
        perror("Błąd!!! Proszę podać plik wejściowy jako pierwszy argument, a plik wyjściowy jako drugi!");
        exit(EXIT_FAILURE);
    }
    
    srand(time(NULL));
    
    text_file_descriptor = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(text_file_descriptor == -1)
    {
        perror("Błąd otwarcia pliku do zapisu");
        exit(EXIT_FAILURE);
    }
    
    potok = open(argv[2], O_RDONLY, 0644);
    if(potok == -1)
    {
        perror("Błąd otwarcia potoku do odczytu");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        sleep(rand()%3);
        
        odczyt = read(potok, &bufor, rozmiarDanychKonsument);
        if(odczyt == -1)
        {
            printf("Błąd oczytu z potoku\n");
            exit(EXIT_FAILURE);
        }
        else if(odczyt == 0)
            break;
        else
        {
            if(write(text_file_descriptor, &bufor, odczyt) == -1)
            {
                perror("Błąd funkcji write");
                exit(EXIT_FAILURE);
            }
            bufor[odczyt] = '\0';
            printf("\n---------------------\nKonsument otrzymal: \n");
            if(write(STDOUT_FILENO, bufor, strlen(bufor)) == -1)
            {
                perror("Błąd funkcji write");
                exit(EXIT_FAILURE);

            }
            printf("\n---------------------\n");
        }
    }
    if(close(text_file_descriptor) == -1)
    {
        perror ("Błąd zamknięcia pliku wyjścia");
        exit(EXIT_FAILURE);
    }
    if(close(potok) == -1)
    {
        perror ("Błąd zamknięcia potoku");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
