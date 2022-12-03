#include <stdio.h>
#include <stdlib.h>

#include <cilk/cilk.h>

#include <time.h>

const int N = 1e8;
int* a = NULL;
int* b = NULL;

void swap(int* a, int* b) {
  int c = *a;
  *a = *b;
  *b = c;
}

void qsort_seq(int l, int r) {
  while (r - l > 1) {
    int pivot = a[r - 1];

    int q = l;
    int p = r - 1;
    
    while (q < p) {
      if (a[q] < pivot) {
        q++;
      } else {
        p--;
        swap(a + p, a + q);
      }
    }
    swap(a + r - 1, a + p);

    qsort_seq(l, p);
    l = p + 1;
  }
}

void qsort_par(int l, int r) {
  if (r - l > 1) {
    int pivot = a[r - 1];

    int q = l;
    int p = r - 1;
    
    while (q < p) {
      if (a[q] < pivot) {
        q++;
      } else {
        p--;
        swap(a + p, a + q);
      }
    }
    swap(a + r - 1, a + p);

    cilk_spawn qsort_par(p + 1, r);
    qsort_par(l, p);
    cilk_sync;
  }
}

void print_array(const int *a, size_t n) {
  printf("a: (%d", a[0]);
  int i;
  for (i = 1; i < n; ++i) {
    printf(", %d", a[i]);
  }
  printf(")\n");
}

int validate() {
  for (int i = 1; i < N; i++) {
    if (a[i] < a[i - 1]) {
      return 0;
    }
  }
  return 1;
}

const int L = 5;

int main(int argc, char *argv[]) {
  srand(time(NULL));

  float speed_up = 0.0;
  a = (int*) malloc(sizeof(int) * N);
  b = (int*) malloc(sizeof(int) * N);
  
  for (int l = 0; l < L; l++) {

    for (int i = 0; i < N; i++) {
      a[i] = rand();
      b[i] = a[i];
    }

    time_t start;
    time_t end;

    time(&start);
    qsort_seq(0, N);
    time(&end);
    float seq_time = difftime(end, start);
    printf("seq time: %f \n", seq_time);
    if (validate() == 0) {
      printf("array is not sorted\n");
      return 0;
    }

    for (int i = 0; i < N; i++) {
      a[i] = b[i];
    }

    time(&start);
    qsort_par(0, N);
    time(&end);
    float par_time = difftime(end, start);
    printf("par time: %f \n", par_time);
    if (validate() == 0) {
      printf("array is not sorted\n");
      return 1;
    }

    speed_up += (seq_time / par_time);
  }

  printf("%f \n", speed_up / L);
  return 0;
}
