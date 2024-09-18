// Compilar com gcc -pthread ex1.c -o ex1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define THINKING_TIME 100000
#define INIT_THREAD_INTERVAL 10000

sem_t sem;

void *funcao(void *arg){
  int t_id = *((int *)arg);
  int valor;

  sem_wait(&sem);  
  sem_getvalue(&sem, &valor);
  printf("thread %d adquiriu o semaforo (valor = %d)\n",t_id, valor);
  usleep(rand() % THINKING_TIME);
  printf("thread %d liberou o semaforo\n",t_id);
  sem_post(&sem);

  return 0;
}

int main (int argc, char **argv){
  if(argc!=2){
	 printf("Digite %s num_threads\n", argv[0]);
	 exit(0);
  }

  int n_threads = atoi(argv[1]);
  int ids[n_threads];
  int i;
  pthread_t threads[n_threads];

  srand(time(NULL));

  sem_init(&sem, 0, 2);

  for(i = 0; i < n_threads; i++){
    ids[i] = i;
    usleep(rand() % INIT_THREAD_INTERVAL);
    pthread_create(&threads[i],NULL,funcao,&ids[i]);
  }

  for(i = 0; i < n_threads; i++){
    pthread_join(threads[i],NULL);
  }

  sem_destroy(&sem);

  return 0;
}
