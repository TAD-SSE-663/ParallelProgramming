#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#ifdef _OPENMP
#	include <omp.h>
#endif

#define BILLION 1E9

void setup(int* num_data, int **ser_data, int **par_data);

int main(int argc, char* argv[])
{
    printf("---    OpenMP Showcase - Sort    ---\n\n");
    struct timespec s_t, e_t;
    double tot_time;
    int num_data;
    int thread_count = strtol(argv[1], NULL, 10);

    int *ser_data, *par_data;
    setup(&num_data, &ser_data, &par_data);

    // Serial sort.
    clock_gettime(CLOCK_MONOTONIC, &s_t);
    int ser_tmp = 0;
    for (int i = 0; i < num_data; i++) {
        if (i % 2 == 0) {
            for (int j = 1; j < num_data; j += 2) {
                if (ser_data[j-1] > ser_data[j]) {
                    ser_tmp = ser_data[j-1];
                    ser_data[j-1] = ser_data[j];
                    ser_data[j] = ser_tmp;
                }
            }
        } else {
            for (int j = 1; j < num_data - 1; j += 2) {
                if (ser_data[j] > ser_data[j+1]) {
                    ser_tmp = ser_data[j+1];
                    ser_data[j+1] = ser_data[j];
                    ser_data[j] = ser_tmp;
                }
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    printf("Serial time: %lf\n", tot_time);


    // Parallel sort.
    clock_gettime(CLOCK_MONOTONIC, &s_t);
    int i, j, par_tmp;
#   pragma omp parallel num_threads(thread_count) default(none) \
        private(i, j, par_tmp) shared(par_data, num_data)
    for (i = 0; i < num_data; i++) {
        if (i % 2 == 0) {
#           pragma omp for
            for (j = 1; j < num_data; j += 2) {
                if (par_data[j-1] > par_data[j]) {
                    par_tmp = par_data[j-1];
                    par_data[j-1] = par_data[j];
                    par_data[j] = par_tmp;
                }
            }
        } else {
#           pragma omp for
            for (j = 1; j < num_data - 1; j += 2) {
                if (par_data[j] > par_data[j+1]) {
                    par_tmp = par_data[j+1];
                    par_data[j+1] = par_data[j];
                    par_data[j] = par_tmp;
                }
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    printf("Parallel sort time: %lf\n", tot_time);

    return 0;
}

void setup(int* num_data, int **ser_data, int **par_data)
{
    printf("Enter num_data: ");
    fflush(stdout);
    scanf("%d", num_data);
    srand(1);
    *ser_data = malloc(sizeof(int) * (*num_data));
    *par_data = malloc(sizeof(int) * (*num_data));
    for (int i = 0; i < (*num_data); i++) {
        (*ser_data)[i] = (rand() % 100) + 1;
        (*par_data)[i] = (*ser_data)[i];
    }
}
