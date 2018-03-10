#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#ifdef _OPENMP
#	include <omp.h>
#endif

#define BILLION 1E9

int thread_count;
int num_data;
int par_sum;
int *data;

void* Sum(void* rank);

int main(int argc, char* argv[])
{
    printf("---    OpenMP Showcase - Hello    ---\n\n");
    struct timespec s_t, e_t;
    double tot_time;

    // Get file information.
    FILE *data_file;
    data_file = fopen(argv[1], "r");
    num_data = strtol(argv[2], NULL, 10);
    char line[10];
    data = malloc(sizeof(int) * num_data);
    int line_index = 0;
    while (line_index < num_data && fgets(line, sizeof(line), data_file) != NULL)
    {
        data[line_index++] = strtol(line, NULL, 10);
    }

    // Serial execution.
    
}