#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* globals -- why? bad! */
int cache_size;
int num_elements;
int num_traversals;
uint64_t *array;
/* globals */

double calc_time(struct timespec start, struct timespec end) {
  double start_sec =
      (double)start.tv_sec * 1000000000.0 + (double)start.tv_nsec;
  double end_sec = (double)end.tv_sec * 1000000000.0 + (double)end.tv_nsec;

  if (end_sec < start_sec) {
    return 0;
  } else {
    return end_sec - start_sec;
  }
}

void init_array_rand() {
  int i, j;
  uint64_t tmp;

  for (i = 0; i < num_elements; i++) {
    array[i] = i;
  }

  i = num_elements;
  while (i > 1) {
    i--;
    j = rand() % i;
    tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
  }
}

void init_array_seq() {
  int i;
  for (i = 0; i < num_elements; i++) {
    array[i] = (i + 1);
  }
  array[(num_elements - 1)] = 0;
}

uint64_t write_only(uint64_t loop_iters) {
  for (uint64_t i = 0; i < loop_iters; i++) {
    array[i] = 100 * i;
  }
  return loop_iters * 8;
}

int main(int argc, char *argv[]) {
  int loop_iters = 0;
  uint64_t data = 0;

  struct timespec start_time, end_time;

  if (argc != 3) {
    printf("Usage: ./cache_test <cache_size(KB)> <mode(1|2|3)>\n"
           "Modes:\n"
           "1: write only workload\n"
           "2: read to write ratio = 1:1\n"
           "3: read to write ratio = 2:1\n");
    return EXIT_SUCCESS;
  }

  cache_size = atoi(argv[1]);
  num_elements = cache_size / (64 * 2);
  num_traversals = atoi(argv[2]);
  array = (uint64_t *)malloc(num_elements * sizeof(uint64_t));

  // initialize array
  init_array_seq();

  loop_iters = num_elements * num_traversals;

  clock_gettime(CLOCK_MONOTONIC, &start_time);
  data = write_only(loop_iters);
  clock_gettime(CLOCK_MONOTONIC, &end_time);

  double elapsed_ns = calc_time(start_time, end_time);

  printf("Time = %f\n", elapsed_ns);
  printf("NS per load = %f\n",
         (elapsed_ns / (((uint64_t)num_elements * (uint64_t)num_traversals))));

  printf("Data = %lu\n", data);

  free(array);
}
