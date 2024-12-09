#ifndef WATKI_H
#define WATKI_H

#include <pthread.h>

pthread_t utworz_watek (void *funkcja(void *), void *argument);
void polacz_watek (pthread_t watek);
void inicjalizuj_mutex (pthread_mutex_t *mutex);
void zamknij_mutex (pthread_mutex_t *mutex);
void otworz_mutex (pthread_mutex_t *mutex);
void usun_mutex (pthread_mutex_t *mutex);

#endif
