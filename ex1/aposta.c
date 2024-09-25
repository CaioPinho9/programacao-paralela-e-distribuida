#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>

int lucky_number;
int n_gamblers;
sem_t gambler_sem;
sem_t auditor_sem;

typedef struct
{
    int id;
    int bet;
    int tries;
    pthread_mutex_t mutex;
    sem_t finish_sem;
} gambler_t;

gambler_t gambler_data;

void *gambler(void *arg)
{
    int id = (intptr_t)arg;
    sem_wait(&gambler_sem);

    int bet;
    int tries = 0;

    printf("Gambler %d is betting...\n", id);

    do
    {
        sleep(rand() % 5);
        bet = rand() % 10;
        tries++;
        printf("Gambler %d tried %d\n", id, bet);
    } while (bet != lucky_number);

    pthread_mutex_lock(&gambler_data.mutex);
    printf("Gambler %d sent %d to auditor\n", id, bet);
    gambler_data.bet = bet;
    gambler_data.tries = tries;
    gambler_data.id = id;
    sem_post(&auditor_sem);
    sem_wait(&gambler_data.finish_sem);
    pthread_mutex_unlock(&gambler_data.mutex);

    return NULL;
}

void *auditor(void *arg)
{
    printf("Auditor is choosing lucky number...\n");
    lucky_number = rand() % 10;

    sleep(rand() % 5);

    printf("Lucky number is %d\n", lucky_number);
    printf("Bets started\n");

    for (int i = 0; i < n_gamblers; i++)
    {
        sem_post(&gambler_sem);
    }

    for (int i = 0; i < n_gamblers; i++)
    {
        sem_wait(&auditor_sem);
        printf("Gambler %d won in %d place with bet %d in %d tries\n", gambler_data.id, i + 1, gambler_data.bet, gambler_data.tries);
        sem_post(&gambler_data.finish_sem);
    }

    printf("Bets finished\n");

    return NULL;
}

int main(int argc, char **argv)
{
    srand(time(NULL));

    if (argc != 2)
    {
        printf("Usage: %s n_gamblers\n", argv[0]);
        return 1;
    }

    n_gamblers = atoi(argv[1]);

    sem_init(&gambler_sem, 0, 0);
    sem_init(&auditor_sem, 0, 0);
    sem_init(&gambler_data.finish_sem, 0, 0);

    pthread_t gamblers_thread[n_gamblers];
    pthread_t auditor_thread;

    for (int i = 0; i < n_gamblers; i++)
    {
        pthread_create(&gamblers_thread[i], NULL, gambler, (void *)(intptr_t)i);
    }

    pthread_create(&auditor_thread, NULL, auditor, NULL);

    for (int i = 0; i < n_gamblers; i++)
    {
        pthread_join(gamblers_thread[i], NULL);
    }

    pthread_join(auditor_thread, NULL);
}