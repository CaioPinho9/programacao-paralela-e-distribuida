// Compilar com gcc -pthread ex5.c -o ex5

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_barrier_t barreira;

void *thread_alo(void* arg) {
	
	sleep(1);
	printf("Alo ");
	fflush(stdout);
	// Aguarda na barreira 
	pthread_barrier_wait(&barreira);
	return 0;
}

void *thread_mundo(void* arg) {
	pthread_barrier_wait(&barreira);
	printf("mundo \n");
	fflush(stdout);
	return 0;
}

int main(int argc, char** argv) {
	pthread_t t1, t2;
	//pthread_t t1, t2, t3;
	
	pthread_barrier_init(&barreira, NULL, 2);
	//pthread_barrier_init(&barreira, NULL, 3);
	
	int r1 = pthread_create(&t1, NULL, thread_alo, NULL);
	int r2 = pthread_create(&t2, NULL, thread_mundo, NULL);
	//int r3 = pthread_create(&t3, NULL, thread_mundo, NULL);

	if (r1 || r2) {
	//if (r1 || r2 || r3) {	
		printf("Threads nao foram criadas\n");
		exit(1);
	}

	r1 = pthread_join(t1, NULL);
	r2 = pthread_join(t2, NULL);
	//r3 = pthread_join(t3, NULL);

	if (r1 || r2) {
	//if (r1 || r2 || r3) {	
		printf("Nao aguarda threads no join\n");
		exit(1);
	}

	pthread_barrier_destroy(&barreira);
	return 0;
}
