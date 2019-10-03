#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* globals -- why? bad! */
int cache_size;
int num_elements;
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
  printf("Write Only\n");
  for (uint64_t i = 0; i < loop_iters; i++) {
    array[i] = 100 * i;
  }
  return loop_iters * 8;
}

uint64_t read_write_1(uint64_t loop_iters) {
  printf("Read Write 1\n");
  for (uint64_t i = 0; i < loop_iters; i++) {
    int j = array[i + 1];
    array[i] = 100 * j;
  }
  return loop_iters * 2 * 8;
}

uint64_t read_write_2(uint64_t loop_iters) {
  printf("Read Write 2\n");
  for (uint64_t i = 0; i < loop_iters; i++) {
    int j = array[i + 1];
    int k = array[i + 2];
    array[i] = 100 * (j + k);
  }
  return loop_iters * 3 * 8;
}

void usage() {
  printf("Usage: ./cache_test <cache_size(KB)> <mode(1|2|3)>\n"
         "Modes:\n"
         "1: write only workload\n"
         "2: read to write ratio = 1:1\n"
         "3: read to write ratio = 2:1\n");
}

int main(int argc, char *argv[]) {
  int loop_iters = 0;
  uint64_t data = 0;
  int mode = 0;
  uint64_t (*func)(uint64_t);

  struct timespec start_time, end_time;

  if (argc != 3) {
    usage();
    return EXIT_SUCCESS;
  }

  cache_size = atoi(argv[1]);
  num_elements = cache_size * 1024 / sizeof(uint64_t);
  mode = atoi(argv[2]);

  switch (mode) {
  case 1:
    func = &write_only;
    break;
  case 2:
    func = &read_write_1;
    break;
  case 3:
    func = &read_write_2;
    break;
  default:
    printf("Invalid mode of operation!");
    usage();
    exit(EXIT_FAILURE);
  }

  array = (uint64_t *)malloc(num_elements * mode * sizeof(uint64_t));

  // initialize array
  init_array_seq();
  printf("array[0] = %lu\n", array[0]);

  loop_iters = num_elements;
  printf("Num of loop iters = %d\n", loop_iters);

  clock_gettime(CLOCK_MONOTONIC, &start_time);
  data = (*func)(loop_iters); // bytes
  clock_gettime(CLOCK_MONOTONIC, &end_time);

  double elapsed_ns = calc_time(start_time, end_time);

  printf("Time(ns) = %f\n", elapsed_ns);
  printf("Time(s) = %f\n", elapsed_ns / 1000000000);
  printf("Num of iters: %d\n", loop_iters);
  printf("NS per load = %f\n", (elapsed_ns / loop_iters));

  printf("Data = %lu\n", data);
  printf("Bandwidth = %.2fGBps\n", (data / elapsed_ns));
  printf("array[0] = %lu\n", array[0]);
  free(array);
}
