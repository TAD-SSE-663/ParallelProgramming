#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#ifdef _OPENMP
#	include <omp.h>
#endif

#define BILLION 1E9

void Sum_Crit(const int[], const int, int*);
int Sum(const int[], const int);

int main(int argc, char* argv[])
{
    printf("---    OpenMP Showcase - Sum    ---\n\n");
    struct timespec s_t, e_t;
    double tot_time;
    int tot_sum;

    // Get file information.
    FILE *data_file;
    data_file = fopen(argv[1], "r");
    int num_data = strtol(argv[2], NULL, 10);
    int thread_count = strtol(argv[3], NULL, 10);
    char line[10];
    int *data = malloc(sizeof(int) * num_data);
    int line_index = 0;
    while (line_index < num_data && fgets(line, sizeof(line), data_file) != NULL)
    {
        data[line_index++] = strtol(line, NULL, 10);
    }

    // Serial execution.
    clock_gettime(CLOCK_MONOTONIC, &s_t);
    tot_sum = 0;
    for (int i = 0; i < num_data; i++)
    {
        tot_sum += data[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    
    printf("Serial sum: %d\n", tot_sum);
    printf("Serial time: %lf\n\n", tot_time);


    // Basic Parallel Function Call.
    clock_gettime(CLOCK_MONOTONIC, &s_t);
    tot_sum = 0;
#   pragma omp parallel num_threads(thread_count)
    Sum_Crit(data, num_data, &tot_sum);
    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    
    printf("Basic Parallel Function sum: %d\n", tot_sum);
    printf("Basic Parallel Function time: %lf\n\n", tot_time);


    // Parallel No Pointer.
    clock_gettime(CLOCK_MONOTONIC, &s_t);
    tot_sum = 0;
#   pragma omp parallel num_threads(thread_count)
    {
        int sum = Sum(data, num_data);
#       pragma omp critical
        tot_sum += sum;
    }
    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    
    printf("Parallel No Pointer sum: %d\n", tot_sum);
    printf("Parallel No Pointer time: %lf\n\n", tot_time);


    // Parallel With For & Reduction.
    clock_gettime(CLOCK_MONOTONIC, &s_t);
    tot_sum = 0;
    int i;
#   pragma omp parallel for num_threads(thread_count) \
        reduction(+:tot_sum) default(none) private(i) \
        shared(num_data, data)
    for (i = 0; i < num_data; i++)
    {
        tot_sum += data[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    
    printf("Parallel For Reduction sum: %d\n", tot_sum);
    printf("Parallel For Reduction time: %lf\n\n", tot_time);


    system("pause");
    return 0;
}

void Sum_Crit(const int data[], const int num_data, int *tot_sum)
{
    int thread_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    int num_loops = num_data / thread_count;
    int start_index = num_loops * thread_rank;
    int sum = 0;
    for (int i = start_index; i < start_index + num_loops; i++)
    {
        sum += data[i];
    }
#   pragma omp critical
    *tot_sum += sum;
}

int Sum(const int data[], const int num_data)
{
    int thread_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    int num_loops = num_data / thread_count;
    int start_index = num_loops * thread_rank;
    int sum = 0;
    for (int i = start_index; i < start_index + num_loops; i++)
    {
        sum += data[i];
    }
    return sum;
}
