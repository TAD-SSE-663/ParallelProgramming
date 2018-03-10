#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#ifdef _OPENMP
#	include <omp.h>
#endif

#define BILLION 1E9

int main(int argc, char* argv[])
{
    printf("---    OpenMP Showcase - Schedule    ---\n\n");
    struct timespec s_t, e_t;
    double tot_time;
    int tot_sum, i;

    // Get file information.
    FILE *data_file;
    data_file = fopen(argv[1], "r");
    int num_data = strtol(argv[2], NULL, 10);
    int thread_count = strtol(argv[3], NULL, 10);
    char line[10];
    int *data = malloc(sizeof(int) * num_data);
    int line_index = 0;
    while (line_index < num_data && fgets(line, sizeof(line), data_file) != NULL) {
        data[line_index++] = strtol(line, NULL, 10);
    }


    // Default Scheduling
    clock_gettime(CLOCK_MONOTONIC, &s_t);
    tot_sum = 0;
#   pragma omp parallel for num_threads(thread_count) \
        reduction(+:tot_sum) default(none) private(i) \
        shared(num_data, data)
    for (i = 0; i < num_data; i++) {
        tot_sum += data[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    
    printf("Default schedule sum: %d\n", tot_sum);
    printf("Default schedule time: %lf\n\n", tot_time);


    // Static Scheduling - 100
    clock_gettime(CLOCK_MONOTONIC, &s_t);
    tot_sum = 0;
#   pragma omp parallel for num_threads(thread_count) \
        reduction(+:tot_sum) default(none) private(i) \
        shared(num_data, data) schedule(static, 100)
    for (i = 0; i < num_data; i++) {
        tot_sum += data[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    
    printf("Static 100 schedule sum: %d\n", tot_sum);
    printf("Static 100 schedule time: %lf\n\n", tot_time);


    // Static Scheduling - 1000
    clock_gettime(CLOCK_MONOTONIC, &s_t);
    tot_sum = 0;
#   pragma omp parallel for num_threads(thread_count) \
        reduction(+:tot_sum) default(none) private(i) \
        shared(num_data, data) schedule(static, 1000)
    for (i = 0; i < num_data; i++) {
        tot_sum += data[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    
    printf("Static 1000 schedule sum: %d\n", tot_sum);
    printf("Static 1000 schedule time: %lf\n\n", tot_time);


    // Dynamic Scheduling - 100
    clock_gettime(CLOCK_MONOTONIC, &s_t);
    tot_sum = 0;
#   pragma omp parallel for num_threads(thread_count) \
        reduction(+:tot_sum) default(none) private(i) \
        shared(num_data, data) schedule(dynamic, 100)
    for (i = 0; i < num_data; i++) {
        tot_sum += data[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    
    printf("Dynamic 100 schedule sum: %d\n", tot_sum);
    printf("Dynamic 100 schedule time: %lf\n\n", tot_time);


    // Dynamic Scheduling - 1000
    clock_gettime(CLOCK_MONOTONIC, &s_t);
    tot_sum = 0;
#   pragma omp parallel for num_threads(thread_count) \
        reduction(+:tot_sum) default(none) private(i) \
        shared(num_data, data) schedule(dynamic, 1000)
    for (i = 0; i < num_data; i++) {
        tot_sum += data[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    
    printf("Dynamic 1000 schedule sum: %d\n", tot_sum);
    printf("Dynamic 1000 schedule time: %lf\n\n", tot_time);


    // Guided Scheduling - 100
    clock_gettime(CLOCK_MONOTONIC, &s_t);
    tot_sum = 0;
#   pragma omp parallel for num_threads(thread_count) \
        reduction(+:tot_sum) default(none) private(i) \
        shared(num_data, data) schedule(guided, 100)
    for (i = 0; i < num_data; i++) {
        tot_sum += data[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    
    printf("Guided 100 schedule sum: %d\n", tot_sum);
    printf("Guided 100 schedule time: %lf\n\n", tot_time);


    // Guided Scheduling - 1000
    clock_gettime(CLOCK_MONOTONIC, &s_t);
    tot_sum = 0;
#   pragma omp parallel for num_threads(thread_count) \
        reduction(+:tot_sum) default(none) private(i) \
        shared(num_data, data) schedule(guided, 1000)
    for (i = 0; i < num_data; i++) {
        tot_sum += data[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    
    printf("Guided 1000 schedule sum: %d\n", tot_sum);
    printf("Guided 1000 schedule time: %lf\n\n", tot_time);

    system("pause");
    return 0;
}
