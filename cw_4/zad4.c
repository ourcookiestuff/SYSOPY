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
    int pipe_file_descriptor[2];
    char bufor[1024];
    const size_t rozmiarDanychProducent = 20;
    const size_t rozmiarDanychKonsument = 10;
    int odczyt;
    int stat_lock;
    
    if(argv[1] == NULL || argv[2] == NULL)
    {
        perror("Błąd!!! Proszę podać plik wejściowy jako pierwszy argument, a plik wyjściowy jako drugi!");
        exit(EXIT_FAILURE);
    }
    
    if(pipe(pipe_file_descriptor) == -1)
    {
        perror("Błąd potoku");
        exit(EXIT_FAILURE);
    }
    
    srand(time(NULL));
    
    switch(fork())
    {
        case -1:
            perror("fork error");
            exit(EXIT_FAILURE);
            
        case 0:
            //akcja dla procesu konsumenta
            
            if(close(pipe_file_descriptor[1]) == -1)
            {
                perror("Błąd zamknięcia potoku");
                exit(EXIT_FAILURE);
            }
            text_file_descriptor = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(text_file_descriptor == -1)
            {
                perror("Błąd otwarcia pliku do zapisu");
                exit(EXIT_FAILURE);
            }
            while(1)
            {
                sleep(rand()%3);
                
                odczyt = read(pipe_file_descriptor[0], &bufor, rozmiarDanychKonsument);
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
            if(close(pipe_file_descriptor[0]) == -1)
            {
                perror ("Błąd zamknięcia potoku");
                exit(EXIT_FAILURE);
            }
            break;
            
        default:
            // akcja dla procesu producenta
            
            if(close(pipe_file_descriptor[0]) == -1)
            {
                perror ("Błąd zamknięcia potoku");
                exit(EXIT_FAILURE);
            }
            text_file_descriptor = open(argv[1], O_RDONLY, 0644);
            if(text_file_descriptor == -1)
            {
                perror("Błąd otwarcia pliku do odczytu");
                exit(EXIT_FAILURE);
            }
            
            while(1)
            {
                sleep(rand()%3);
                
                odczyt = read(text_file_descriptor, &bufor, rozmiarDanychProducent);
                if(odczyt == -1)
                {
                    printf("Błąd oczytu z pliku\n");
                    exit(EXIT_FAILURE);
                }
                else if(odczyt == 0)
                    break;
                else
                {
                    if(write(pipe_file_descriptor[1], &bufor, odczyt) == -1)
                    {
                        perror("Błąd funkcji write");
                        exit(EXIT_FAILURE);

                    }
                    bufor[odczyt] = '\0';
                    printf("\n---------------------\nProducent wyslal: \n");
                    if(write (STDOUT_FILENO, bufor, strlen(bufor)) == -1)
                    {
                        perror("Błąd funkcji write");
                        exit(EXIT_FAILURE);

                    }
                    printf("\n---------------------\n");
                }
            }
            if (close(text_file_descriptor) == -1)
            {
                perror ("Błąd zamknięcia pliku wyjścia");
                exit (EXIT_FAILURE);
            }
            
            if (close(pipe_file_descriptor[1]) == -1)
            {
                perror ("Błąd zamknięcia potoku");
                exit (EXIT_FAILURE);
            }
            
            if (wait (&stat_lock) == -1) {
                perror ("Błąd wait");
                exit (EXIT_FAILURE);
            }
            break;
    }
    return 0;
}

