#include <stdlib.h>
#include <stdio.h>
#ifdef _OPENMP
#	include <omp.h>
#endif

#define BILLION 1E9

int main(int argc, char* argv[])
{
    printf("---    OpenMP Showcase - Messages    ---\n\n");

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


    system("pause");
    return 0;
}
