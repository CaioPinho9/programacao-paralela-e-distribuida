#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

// #define MAX_NUMBERS 10
#define MAX_NUMBERS 500000000
#define MAX_VALUE 1000

#define NUM_THREADS 3

float numbers[MAX_NUMBERS];

void init_numbers()
{
  unsigned int seed;
  for (unsigned int i = 0; i < MAX_NUMBERS;)
  {
    numbers[i] = ((float)rand_r(&seed) / (float)RAND_MAX) * MAX_VALUE;
  }
}

void *run_operation(void *arg)
{
  int num = *((int *)arg);
  for (unsigned int i = num; i < MAX_NUMBERS; i += NUM_THREADS)
  {
    numbers[i] = numbers[i] * 0.2 + numbers[i] / 0.3;
  }
}

// int show_numbers()
// {
//   for (unsigned int i = 0; i < MAX_NUMBERS; i++)
//   {
//     printf("number[%u] = %f\n", i, numbers[i]);
//   }
//   return 0;
// }

int main(int argc, char **argv)
{
  srand(time(NULL));

  struct timeval t1, t2;
  gettimeofday(&t1, NULL);

  init_numbers();

  gettimeofday(&t2, NULL);
  double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);
  printf("tempo total = %f\n", t_total);

  gettimeofday(&t1, NULL);
  pthread_t threads1[NUM_THREADS];
  for (size_t i = 0; i < NUM_THREADS; i++)
  {
    size_t *index = malloc(sizeof(size_t)); // Allocate memory for each index
    *index = i;
    if (pthread_create(&threads1[i], NULL, run_operation, index) != 0)
    {
      perror("Failed to create thread");
      return 1;
    }
  }
  for (size_t i = 0; i < NUM_THREADS; i++)
  {
    if (pthread_join(threads1[i], NULL) != 0)
    {
      perror("Failed to join thread");
      return 1;
    }
  }
  gettimeofday(&t2, NULL);
  t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);
  printf("tempo total = %f\n", t_total);

  return 0;
}
