#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int n_threads;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} barrier_t;

void barrier_init(barrier_t *barrier, int count)
{
    barrier->count = 0;
    barrier->n_threads = count;
    pthread_mutex_init(&barrier->mutex, NULL);
    pthread_cond_init(&barrier->cond, NULL);
}

void espera(barrier_t *barrier)
{
    pthread_mutex_lock(&barrier->mutex);
    barrier->count++;
    while (barrier->count < barrier->n_threads)
    {
        printf("Thread %ld esperando na barreira waiting %d\n", pthread_self(), barrier->count);
        pthread_cond_wait(&barrier->cond, &barrier->mutex);
    }
    pthread_mutex_unlock(&barrier->mutex);
    pthread_cond_broadcast(&barrier->cond);
}

void *esperar(void *arg)
{
    barrier_t *barrier = (barrier_t *)arg;
    printf("Thread %ld inicializada\n", pthread_self());
    sleep(rand() % 5);
    espera(barrier);
    printf("Thread %ld esperou na barreira\n", pthread_self());
    return NULL;
}

int main(int argc, char **argv)
{
    srand(time(NULL));

    if (argc != 2)
    {
        printf("Usage: %s n_threads\n", argv[0]);
        return 1;
    }

    int n_threads = atoi(argv[1]);

    barrier_t barrier;
    barrier_init(&barrier, n_threads);

    pthread_t threads[n_threads];
    for (int i = 0; i < n_threads; i++)
    {
        pthread_create(&threads[i], NULL, esperar, &barrier);
    }

    for (int i = 0; i < n_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }
}