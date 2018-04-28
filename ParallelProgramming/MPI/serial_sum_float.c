#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define BILLION 1E9

int main(int argc, char* argv[])
{
    struct timespec s_t, e_t;
    double tot_time;
    float tot_sum;

    // Get file information.
    FILE *data_file;
    int num_data = strtol(argv[1], NULL, 10);
    data_file = fopen(argv[2], "r");
    char line[10];
    float *data = malloc(sizeof(float) * num_data);
    int line_index = 0;
    while (line_index < num_data && fgets(line, sizeof(line), data_file) != NULL) {
        data[line_index++] = strtol(line, NULL, 10);
    }

    // Serial execution.
    clock_gettime(CLOCK_MONOTONIC, &s_t);
    tot_sum = 0;
    for (int i = 0; i < num_data; i++) {
        tot_sum += data[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    
    printf("Serial sum: %f\n", tot_sum);
    printf("Serial time: %lf\n\n", tot_time);

    return 0;
}