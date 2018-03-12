#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

void get_num_data(int rank, int num_processes, int* num_data, int* num_loops);
void get_file_input(int num_data, int** data);

int main()
{
	int num_processes, process_rank, num_data, num_loops;
    int *local_data;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    // Code of interest.
    get_num_data(process_rank, num_processes, &num_data, &num_loops);

    int* tot_data = NULL;
    if (process_rank == 0) {
        get_file_input(num_data, &tot_data);
    }
    local_data = malloc(num_loops * sizeof(int));
    MPI_Barrier(MPI_COMM_WORLD);

    if (process_rank == 0) {
        MPI_Scatter(tot_data, num_loops, MPI_INT, local_data, 
            num_loops, MPI_INT, 0, MPI_COMM_WORLD);
        free(tot_data);
    } else {
        MPI_Scatter(tot_data, num_loops, MPI_INT, local_data, 
            num_loops, MPI_INT, 0, MPI_COMM_WORLD);
    }

    int total_sum, local_sum = 0;
    for (int i = 0; i < num_loops; i++) {
        local_sum += local_data[i];
    }
    free(local_data);
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0,
        MPI_COMM_WORLD);
    
    if (process_rank == 0) {
        printf("Total sum:   %d.\n", total_sum);
    }

    MPI_Finalize();
    return 0;
}

void get_num_data(int rank, int num_processes, int* num_data, int* num_loops)
{
    if (rank == 0) {
        printf("Enter num_data: ");
        fflush(stdout);
        scanf("%d", num_data);
        *num_loops = (*num_data) / num_processes;
    }
    // Start time measurement.
    double start, finish, elapsed, min_elapsed;
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    MPI_Bcast(num_data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(num_loops, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Finsh time measurement.
    finish = MPI_Wtime();
    elapsed = finish - start;
    MPI_Reduce(&elapsed, &min_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0,
        MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total elapsed time: %lf", min_elapsed);
    }
}

void get_file_input(int num_data, int** data)
{
    char file_name[20];
    printf("\nEnter file_name: ", file_name);
    fflush(stdout);
    scanf("%s", file_name);
    FILE *data_file = fopen(file_name, "r");
    char line[10];
    *data = malloc(sizeof(int) * num_data);
    int line_index = 0;
    while (line_index < num_data && fgets(line, sizeof(line), data_file) != NULL) {
        (*data)[line_index++] = strtol(line, NULL, 10);
    }
}
