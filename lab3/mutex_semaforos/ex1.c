// Compilar com gcc -pthread ex1.c -o ex1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THINKING_TIME 100000

pthread_mutex_t mutex;
int compartilhada;

void *funcao(void *arg){
  int t_id = *((int *)arg);

  pthread_mutex_lock(&mutex);
  printf("thread %d adquiriu o mutex\n",t_id);
  compartilhada = t_id;
  usleep(rand() % THINKING_TIME);
  printf("compartilhada = %d\n",compartilhada);
  
  printf("thread %d liberou o mutex\n",t_id);
  pthread_mutex_unlock(&mutex);

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

  pthread_mutex_init(&mutex, NULL);

  for(i = 0; i < n_threads; i++){
    ids[i] = i;
    pthread_create(&threads[i],NULL,funcao,&ids[i]);
  }

  for(i = 0; i < n_threads; i++){
    pthread_join(threads[i],NULL);
  }

  pthread_mutex_destroy(&mutex);

  return 0;
}
