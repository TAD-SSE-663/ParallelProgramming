#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#ifdef _OPENMP
#	include <omp.h>
#endif

#define BILLION 1E9

const int MIN = 0;
const int MAX = 100;
const int NUM_BINS = 10;

void setup(int* num_data, int **ser_data, int **par_data);
int get_bin(int datum, int *bin_max);

int main(int argc, char* argv[])
{
    printf("---    OpenMP Showcase - Histogram    ---\n\n");
    struct timespec s_t, e_t;
    double tot_time;
    int num_data;
    int thread_count = strtol(argv[1], NULL, 10);

    int *ser_data, *par_data;
    setup(&num_data, &ser_data, &par_data);

    clock_gettime(CLOCK_MONOTONIC, &s_t);
    int bin_width = (MAX - MIN) / NUM_BINS;
    int *bin_max = malloc(sizeof(int) * NUM_BINS);
    for (int i = 0; i < NUM_BINS; i++) {
        bin_max[i] = MIN + (bin_width * (i + 1));
    }

    int *bin_counts = calloc(NUM_BINS, sizeof(int));
    for (int i = 0; i < num_data; i++) {
        bin_counts[get_bin(ser_data[i], bin_max)]++;
    }

    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    
    printf("\t\t\t\t\t------      Serial Histogram      ------\n\n");
    for (int i = 0; i < NUM_BINS; i++) {
        printf("%d\t", bin_counts[i]);
    }
    printf("\nSerial Function time: %lf\n\n", tot_time);



    clock_gettime(CLOCK_MONOTONIC, &s_t);
    int pbin_width = (MAX - MIN) / NUM_BINS;
    int *pbin_max = malloc(sizeof(int) * NUM_BINS);
    for (int i = 0; i < NUM_BINS; i++) {
        pbin_max[i] = MIN + (pbin_width * (i + 1));
    }

    int *pbin_counts = calloc(NUM_BINS, sizeof(int));
#   pragma omp parallel for num_threads(thread_count)
    for (int i = 0; i < num_data; i++) {
#       pragma omp atomic
        pbin_counts[get_bin(par_data[i], pbin_max)]++;
    }

    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    
    printf("\t\t\t\t\t------      Parallel Histogram      ------\n\n");
    for (int i = 0; i < NUM_BINS; i++) {
        printf("%d\t", bin_counts[i]);
    }
    printf("\nParallel Function time: %lf\n\n", tot_time);

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
        (*ser_data)[i] = (rand() % 101);
        (*par_data)[i] = (*ser_data)[i];
    }
}

int get_bin(int datum, int *bin_max)
{
    int bin_num = 0;
    for (int i = 0; i < NUM_BINS; i++) {
        if (datum < bin_max[i]) {
            bin_num = i;
            break;
        }
    }
    return bin_num;
}
