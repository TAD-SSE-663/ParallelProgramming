#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

void get_num_data(int rank, int* num_data);
void get_file_input(int num_data, float** data);

int main()
{
    double start, finish, elapsed, min_elapsed;
	int num_processes, process_rank, num_data, num_loops;
    float *local_data;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    get_num_data(process_rank, &num_data);
    
    num_loops = num_data / num_processes;

    float* tot_data = NULL;
    if (process_rank == 0) {
        get_file_input(num_data, &tot_data);
    }
    local_data = malloc(num_loops * sizeof(float));
    MPI_Barrier(MPI_COMM_WORLD);

    if (process_rank == 0) {
        MPI_Scatter(tot_data, num_loops, MPI_FLOAT, local_data, 
            num_loops, MPI_FLOAT, 0, MPI_COMM_WORLD);
        free(tot_data);
    } else {
        MPI_Scatter(tot_data, num_loops, MPI_FLOAT, local_data, 
            num_loops, MPI_FLOAT, 0, MPI_COMM_WORLD);
    }

    // Start time measurement.
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    float total_sum, local_sum = 0;
    for (int i = 0; i < num_loops; i++) {
        local_sum += local_data[i];
    }
    free(local_data);
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_FLOAT, MPI_SUM, 0,
        MPI_COMM_WORLD);
    
    // Finsh time measurement.
    finish = MPI_Wtime();
    elapsed = finish - start;
    MPI_Reduce(&elapsed, &min_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0,
        MPI_COMM_WORLD);

    if (process_rank == 0) {
        printf("Total sum:   %f.\n", total_sum);
        printf("Total elapsed time: %lf", min_elapsed);
    }

    MPI_Finalize();
    return 0;
}

void get_num_data(int rank, int* num_data)
{
    if (rank == 0) {
        printf("Enter num_data: ");
        fflush(stdout);
        scanf("%d", num_data);
    }
    MPI_Bcast(num_data, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

void get_file_input(int num_data, float** data)
{
    char file_name[20];
    printf("\nEnter file_name: ", file_name);
    fflush(stdout);
    scanf("%s", file_name);
    FILE *data_file = fopen(file_name, "r");
    char line[10];
    *data = malloc(sizeof(float) * num_data);
    int line_index = 0;
    while (line_index < num_data && fgets(line, sizeof(line), data_file) != NULL) {
        (*data)[line_index++] = strtol(line, NULL, 10);
    }
}
