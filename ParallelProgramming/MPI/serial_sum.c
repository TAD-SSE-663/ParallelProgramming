#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define BILLION 1E9

int main(int argc, char* argv[])
{
    srand(1);
    int num_data = strtol(argv[1], NULL, 10);
    int *data = malloc(sizeof(int) * num_data);
    for (int i = 0; i < num_data; i++) {
        data[i] = (rand() % 100) + 1;
    }
    
    struct timespec s_t, e_t;
    double tot_time;
    int tot_sum;

    // Serial execution.
    clock_gettime(CLOCK_MONOTONIC, &s_t);
    tot_sum = 0;
    for (int i = 0; i < num_data; i++) {
        tot_sum += data[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    
    printf("Serial sum: %d\n", tot_sum);
    printf("Serial time: %lf\n\n", tot_time);

    return 0;
}