#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

// djb2 hash (referencia: http://www.cse.yorku.ca/~oz/hash.html)
unsigned long hash_djb2(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    return hash;
}

// Converte a hash para string com representacao hexadecimal (para legibilide)
void htos(unsigned long hash, char output[65])
{
    sprintf(output, "%lx", hash);
}

// Verifica se a hash termina com num_zeros numeros zeros
int check_hash_zeros(char *hash_str, int num_zeros)
{
    int len = strlen(hash_str);
    for (int i = len - num_zeros; i < len; i++)
    {
        if (hash_str[i] != '0')
        {
            return 0;
        }
    }
    return 1;
}

void PoW_single_thread(char *data, int num_zeros)
{
    unsigned long hash;
    char hash_str[65];
    long long nonce = 0;
    char buffer[256];

    // Minerador alterar o valor de nonce ate encontrar a hash que satisfaca o desafio
    while (1)
    {

        // Concatena o dado do bloco com o nonce
        sprintf(buffer, "%s%lld", data, nonce);

        // computa a hash do string concatenado (bloco + nonce)
        hash = hash_djb2((unsigned char *)buffer);

        // Para visualizar a hash em formato hexadecimal
        htos(hash, hash_str);

        if (check_hash_zeros(hash_str, num_zeros))
        {
            printf("Sucesso! Valor do nonce: %lld\n", nonce);
            printf("Hash: %s\n", hash_str);
            break;
        }
        nonce++;
    }
}

struct miner_t
{
    /* data */
    char *data;
    int num_zeros;
    int thread_id;
};

#define NUM_THREADS 8
int cancel_threads = 1;

void *PoW_multithread(void *arg)
{
    struct miner_t *miner = (struct miner_t *)arg;
    char *data = miner->data;
    int num_zeros = miner->num_zeros;
    int thread_id = miner->thread_id;

    unsigned long hash;
    char hash_str[65];
    long long nonce = thread_id;
    char buffer[256];

    // Minerador alterar o valor de nonce ate encontrar a hash que satisfaca o desafio
    while (cancel_threads)
    {

        // Concatena o dado do bloco com o nonce
        sprintf(buffer, "%s%lld", data, nonce);

        // computa a hash do string concatenado (bloco + nonce)
        hash = hash_djb2((unsigned char *)buffer);

        // Para visualizar a hash em formato hexadecimal
        htos(hash, hash_str);

        if (check_hash_zeros(hash_str, num_zeros))
        {
            printf("Sucesso! Valor do nonce: %lld\n", nonce);
            printf("Hash: %s\n", hash_str);
            cancel_threads = 0;
            break;
        }
        nonce += NUM_THREADS;
    }
}

void create_threads(char data[], int num_zeros)
{
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];
    cancel_threads = 1;

    for (int i = 0; i < NUM_THREADS; i++)
    {
        ids[i] = i;
        struct miner_t *miner = malloc(sizeof(struct miner_t));
        miner->data = data;
        miner->num_zeros = num_zeros;
        miner->thread_id = ids[i];
        pthread_create(&threads[i], NULL, PoW_multithread, (void *)miner);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

#define NUM_SIMULATIONS 100

int main(int argc, char **argv)
{
    double mean = 0;

    char data[] = "Exemplo de dados do bloco"; // Representa o conteudo do bloco
    int num_zeros;                             // Numero de zeros requeridos no final do hash

    if (argc != 2)
    {
        printf("Digite: %s num_zeros\n", argv[0]);
        return 1;
    }

    num_zeros = atoi(argv[1]);

    printf("Executando PoW ...\n");

    for (int i = 0; i < NUM_SIMULATIONS; i++)
    {
        struct timeval t1, t2;
        gettimeofday(&t1, NULL);
        // create_threads(data, num_zeros);

        PoW_single_thread(data, num_zeros);

        gettimeofday(&t2, NULL);
        double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);

        mean = (mean * i + t_total) / (i + 1);

        printf("rolling mean = %f\n", mean);
    }

    return 0;
}