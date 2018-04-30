#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>

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

    // Serial execution.

    MPI_Init(NULL, NULL);
    double start, finish, elapsed, min_elapsed;
    start = MPI_Wtime();

    //clock_gettime(CLOCK_MONOTONIC, &s_t);
    int sum = 0;
    for (int i = 0; i < num_data; i++) {
        sum += data[i];
    }
    double avg = (double)sum / (double)num_data;
    // clock_gettime(CLOCK_MONOTONIC, &e_t);
    // tot_time = (e_t.tv_sec - s_t.tv_sec) +
    //     (e_t.tv_nsec - s_t.tv_nsec) / BILLION;
    finish = MPI_Wtime();
    elapsed = finish - start;
    MPI_Reduce(&elapsed, &min_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0,
        MPI_COMM_WORLD);
    MPI_Finalize();
    
    printf("Serial avg: %f\n", avg);
    // printf("Serial time: %f\n\n", tot_time);
    printf("Serial time: %f\n\n", elapsed);

    return 0;
}