// Compilar com gcc -pthread ex4.c -o ex4

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

typedef struct {
	int pronto;					// TRUE (1) ou FALSE (0)
	pthread_mutex_t mtx;		// mutex para proteger a regiao critica
	pthread_cond_t var_cond;	// variavel de condicao para sincronizar as duas threads
} estado_compartilhado_t;

void variavel_cond_init(estado_compartilhado_t *estado) {
	estado->pronto = FALSE;
	pthread_mutex_init(&estado->mtx, NULL);
	pthread_cond_init(&estado->var_cond, NULL);
}

void variavel_cond_destroy(estado_compartilhado_t *estado) {
	pthread_mutex_destroy(&estado->mtx);
	pthread_cond_destroy(&estado->var_cond);
}

void *thread_alo(void* arg) {
	estado_compartilhado_t *var = (estado_compartilhado_t*)arg;
	
	//sleep(1);
	pthread_mutex_lock(&var->mtx);
	printf("Alo ");
	
	var->pronto = TRUE;
	// Sinaliza as threads esperando pela variavel de condicao
	//pthread_cond_signal(&var->var_cond);
	pthread_cond_broadcast(&var->var_cond);
	pthread_mutex_unlock(&var->mtx);
	return 0;
}

void *thread_mundo(void* arg) {
	estado_compartilhado_t *var = (estado_compartilhado_t*)arg;
	pthread_mutex_lock(&var->mtx);
	while (!var->pronto) {								// Aguarda a flag ser TRUE
		pthread_cond_wait(&var->var_cond, &var->mtx);	// Aguarda na variável de condição
	}
	printf("mundo \n");
	pthread_mutex_unlock(&var->mtx);
	return 0;
}

int main(int argc, char** argv) {
	estado_compartilhado_t compartilhada;
	variavel_cond_init(&compartilhada);
	pthread_t t1, t2;
	//pthread_t t1, t2, t3;
	
	int r1 = pthread_create(&t1, NULL, thread_alo, &compartilhada);
	int r2 = pthread_create(&t2, NULL, thread_mundo, &compartilhada);
	//int r3 = pthread_create(&t3, NULL, thread_mundo, &compartilhada);

	//if (r1 || r2 || r3) {
	if (r1 || r2) {	
		printf("Threads nao foram criadas\n");
		exit(1);
	}

	r1 = pthread_join(t1, NULL);
	r2 = pthread_join(t2, NULL);
	//r3 = pthread_join(t3, NULL);

	//if (r1 || r2 || r3) {
	if (r1 || r2) {	
		printf("Nao aguarda threads no join\n");
		exit(1);
	}

	variavel_cond_destroy(&compartilhada);
	return 0;
}
