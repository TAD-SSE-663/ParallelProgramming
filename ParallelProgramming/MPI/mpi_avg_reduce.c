#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>

void setup(int rank, int* num_data, int **data);

int main()
{
    double start, finish, elapsed, min_elapsed;
	int num_processes, process_rank, num_data, num_loops;
    int *tot_data, *local_data;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    
    setup(process_rank, &num_data, &tot_data);
    num_loops = num_data / num_processes;
    local_data = malloc(num_loops * sizeof(int));
    MPI_Barrier(MPI_COMM_WORLD);

    // Start time measurement.
    start = MPI_Wtime();

    if (process_rank == 0) {
        MPI_Scatter(tot_data, num_loops, MPI_INT, local_data, 
            num_loops, MPI_INT, 0, MPI_COMM_WORLD);
        free(tot_data);
    } else {
        MPI_Scatter(tot_data, num_loops, MPI_INT, local_data, 
            num_loops, MPI_INT, 0, MPI_COMM_WORLD);
    }

    double total, avg;
    int local_sum = 0;
    for (int i = 0; i < num_loops; i++) {
        local_sum += local_data[i];
    }
    free(local_data);
    double local_avg = (double)local_sum / (double)num_loops;
    MPI_Reduce(&local_avg, &total, 1, MPI_DOUBLE, MPI_SUM, 0,
        MPI_COMM_WORLD);
    if (process_rank == 0) {
        avg = total / (double)num_processes;
    }
    
    // Finsh time measurement.
    finish = MPI_Wtime();
    elapsed = finish - start;
    MPI_Reduce(&elapsed, &min_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0,
        MPI_COMM_WORLD);

    if (process_rank == 0) {
        printf("Average of input:   %f\n", avg);
        printf("Total elapsed time: %f", min_elapsed);
    }

    MPI_Finalize();
    return 0;
}

void setup(int rank, int* num_data, int **data)
{
    if (rank == 0) {
        printf("Enter num_data: ");
        fflush(stdout);
        scanf("%d", num_data);
        srand(1);
        *data = malloc(sizeof(int) * (*num_data));
        for (int i = 0; i < (*num_data); i++) {
            (*data)[i] = (rand() % 100) + 1;
        }
    }
    MPI_Bcast(num_data, 1, MPI_INT, 0, MPI_COMM_WORLD);
}
