#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define UNUSED(x) (void)(x)
// suppress unused var warnings without turning off feature entirely
#define N 1024

/* globals -- why? bad! */
uint64_t loop_tile = 512;
int verbose = 0;
double *A[N], *B[N], *C[N];
/* globals */

double calc_time(struct timespec start, struct timespec end) {
  double start_sec =
      (double)start.tv_sec * 1000000000.0 + (double)start.tv_nsec;
  // printf("start sec = %lf\n", start_sec);
  double end_sec = (double)end.tv_sec * 1000000000.0 + (double)end.tv_nsec;
  // printf("end sec = %lf\n", end_sec);

  if (end_sec < start_sec) {
    return 0;
  } else {
    return end_sec - start_sec;
  }
}

void init_A() {
  if (verbose)
    printf("In init B...\n");
  for (int i = 0; i < N; i++) {
    A[i] = (double *)malloc(N * sizeof(double));
    memset(A[i], 0, N * sizeof(double));
    for (uint64_t j = 0; j < N; j++) {
      A[i][j] = 1;
    }
  }
}

void init_B() {
  if (verbose)
    printf("In init B...\n");
  for (int i = 0; i < N; i++) {
    B[i] = malloc(N * sizeof(double));
    memset(B[i], 0, N * sizeof(double));
    for (uint64_t j = 0; j < N; j++) {
      B[i][j] = 2;
    }
  }
}
void init_C() {
  if (verbose)
    printf("In init C...\n");
  for (int i = 0; i < N; i++) {
    C[i] = malloc(N * sizeof(double));
    memset(C[i], 0, N * sizeof(double));
  }
}

void k_inner() {
  double sum;
  for (uint64_t i = 0; i < N; i++) {
    for (uint64_t j = 0; j < N; j++) {
      sum = 0;
      for (uint64_t k = 0; k < N; k++) {
        sum += A[i][k] * B[k][j];
      }
      C[i][j] = sum;
    }
  }
}

// theoretical best
void j_inner() {
  double tmp_a;
  for (uint64_t k = 0; k < N; k++) {
    for (uint64_t i = 0; i < N; i++) {
      tmp_a = A[i][k];
      for (uint64_t j = 0; j < N; j++) {
        C[i][j] += tmp_a * B[k][j];
      }
    }
  }
}

// theoretical worst
void i_inner() {
  double tmp_b;
  for (uint64_t k = 0; k < N; k++) {
    for (uint64_t j = 0; j < N; j++) {
      tmp_b = B[k][j];
      for (uint64_t i = 0; i < N; i++) {
        C[i][j] += A[i][k] * tmp_b;
      }
    }
  }
}

// theoretical best w/ loop tiling
void j_inner_loop_tiled() {
  if (verbose) {
    printf("sizeof double: %lu\n", (sizeof(double)));
  }
  double tmp_a;
  //  printf("outermost\n");
  for (uint64_t k = 0; k < N; k++) {
    // printf("Outer k: %lu\n", k);
    for (uint64_t i = 0; i < N; i += loop_tile) {
      // printf("Outer i: %lu\n", i);
      for (uint64_t j = 0; j < N; j += loop_tile) {
        // printf("Outer j:%lu\n", j);
        //        for (uint64_t kk = 0; kk < (k + loop_tile); kk++) {
        // printf("Inner k: %lu\n", kk);
        for (uint64_t ii = 0; ii < (i + loop_tile); ii++) {
          // printf("Inner i: %lu\n", ii);
          tmp_a = A[ii][k];
          for (uint64_t jj = 0; jj < (j + loop_tile); jj++) {
            // printf("Inner j: %lu\n", jj);
            C[i][j] += tmp_a * B[k][jj];
          }
        }
      }
    }
  }
}

void print_arr(double **array) {
  printf("arr:\n");
  for (uint64_t i = 0; i < N; i++) {
    for (uint64_t j = 0; j < N; j++) {
      printf("%.0f ", array[i][j]);
    }
    printf("\n");
  }
}

void usage() {
  printf("Usage: ./cache_test <mode(1|2|3|4)>\n"
         "Modes:\n"
         "1: J inner(theoretical best): I-K-J\n"
         "2: K inner: I-J-K\n"
         "3: I inner(theoretical worst): J-K-I\n"
         "4: J inner w/ loop tiling: I-K-J\n");
}

int main(int argc, char *argv[]) {
  int mode = 0;
  void (*func)(void);

  if (verbose)
    printf("In main...\n");
  struct timespec start_time, end_time;

  if (argc != 2) {
    usage();
    return EXIT_SUCCESS;
  }

  mode = atoi(argv[1]);

  switch (mode) {
  case 1:
    func = &j_inner;
    break;
  case 2:
    func = &k_inner;
    break;
  case 3:
    func = &i_inner;
    break;
  case 4:
    func = &j_inner_loop_tiled;
    break;
  default:
    printf("Invalid mode of operation!");
    usage();
    exit(EXIT_FAILURE);
  }

  if (verbose) {
    printf("Starting inits...\n");
  }
  // initialize A
  init_A();
  init_B();
  init_C();

  if (verbose) {
    printf("Ended inits...\n");
    print_arr(A);
    print_arr(B);
    print_arr(C);

    printf("Starting mult...\n");
  }
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  func();
  clock_gettime(CLOCK_MONOTONIC, &end_time);
  if (verbose) {
    printf("Ended mult...\n");

    print_arr(C);
  }

  double elapsed_ns = calc_time(start_time, end_time);

  // printf("Time(ns) = %f\n", elapsed_ns);
  printf("Time = %f sec\n", elapsed_ns / 1000000000);
  // printf("NS per load = %f\n", (elapsed_ns / loop_iters));

  // free A, B, C
}
