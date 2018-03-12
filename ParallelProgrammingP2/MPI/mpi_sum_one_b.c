#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

void get_num_data(int rank, int num_processes, int* num_data, int* num_loops);
void build_mpi_type(int* num_data, int* num_loops, MPI_Datatype*);
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

    MPI_Datatype i_mpi_t;
    build_mpi_type(num_data, num_loops, &i_mpi_t);

    MPI_Bcast(num_data, 1, i_mpi_t, 0, MPI_COMM_WORLD);
    MPI_Type_free(&i_mpi_t);

    // Finsh time measurement.
    finish = MPI_Wtime();
    elapsed = finish - start;
    MPI_Reduce(&elapsed, &min_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0,
        MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total elapsed time: %lf", min_elapsed);
    }
}

void build_mpi_type(int* num_data, int* num_loops, MPI_Datatype* i_mpi_t)
{
    int array_of_blocklengths[2] = {1, 1};
    MPI_Datatype array_of_types[3] = {MPI_INT, MPI_INT};
    MPI_Aint num_data_addr, num_loops_addr;
    MPI_Aint array_of_displacements[2] = {0};

    MPI_Get_address(num_data, &num_data_addr);
    MPI_Get_address(num_loops, &num_loops_addr);
    array_of_displacements[1] = num_loops_addr - num_data_addr;
    MPI_Type_create_struct(2, array_of_blocklengths, 
        array_of_displacements, array_of_types, i_mpi_t);
    MPI_Type_commit(i_mpi_t);
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
