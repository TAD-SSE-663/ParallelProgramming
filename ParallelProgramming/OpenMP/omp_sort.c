#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#ifdef _OPENMP
#	include <omp.h>
#endif

#define BILLION 1E9

int main(int argc, char* argv[])
{
    printf("---    OpenMP Showcase - Sort    ---\n\n");
    struct timespec s_t, e_t;
    double tot_time;

    // Get file information.
    FILE *data_file;
    data_file = fopen(argv[1], "r");
    int num_data = strtol(argv[2], NULL, 10);
    int thread_count = strtol(argv[3], NULL, 10);
    char line[10];
    int *ser_data = malloc(sizeof(int) * num_data);
    int *par_data = malloc(sizeof(int) * num_data);
    int line_index = 0;
    while (line_index < num_data && fgets(line, sizeof(line), data_file) != NULL) {
        ser_data[line_index] = strtol(line, NULL, 10);
        par_data[line_index++] = strtol(line, NULL, 10);
    }

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
    printf("\nSerial time: %lf\n\n", tot_time);


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
    printf("\nParallel sort time: %lf\n\n", tot_time);


    system("pause");
    return 0;
}
