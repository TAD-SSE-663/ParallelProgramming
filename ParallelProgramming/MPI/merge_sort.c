#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

void setup(int rank, int *num_data, int **data);
void merge_sort(int **data, int **tmp_data, int left, int right);
void merge(int **data, int **tmp_data, int left_pos, int right_pos, int right_end);

int main()
{
    double start, finish, elapsed, min_elapsed;
	int num_processes, process_rank, num_data;
    int *scounts, *displcmts, *total_data, *local_data;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    scounts = malloc(sizeof(int) * num_processes);
    displcmts = malloc(sizeof(int) * num_processes);

    setup(process_rank, &num_data, &total_data);
    int curr_displacement = 0;
    int remainder = num_data % num_processes;
    for (int i = 0; i < num_processes; i++) {
        scounts[i] = num_data / num_processes;
        if (remainder > 0) {
            scounts[i]++;
            remainder--;
        }
        displcmts[i] = curr_displacement;
        curr_displacement += scounts[i];
    }
    local_data = malloc(scounts[process_rank] * sizeof(int));

    MPI_Scatterv(total_data, scounts, displcmts, MPI_INT, local_data,
        scounts[process_rank], MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < num_data; i++) printf("%d   \n", local_data[i]);
    fflush(stdout);

    // Start time measurement.
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    int *tmp_local = malloc(sizeof(local_data));
    merge_sort(&local_data, &tmp_local, 0, scounts[process_rank] - 1);

    MPI_Gatherv(local_data, scounts[process_rank], MPI_INT, total_data,
        scounts, displcmts, MPI_INT, 0, MPI_COMM_WORLD);

    if (process_rank == 0) {
        int *tmp_total = malloc(sizeof(total_data));
        merge_sort(&total_data, &tmp_total, 0, num_data - 1);
    }

    // Finsh time measurement.
    finish = MPI_Wtime();
    elapsed = finish - start;
    MPI_Reduce(&elapsed, &min_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0,
        MPI_COMM_WORLD);

    if (process_rank == 0) {
        for (int i = 0; i < num_data; i++) {
            printf("%d  ", total_data[i]);
        }
        printf("Total elapsed time: %f\n", min_elapsed);
    }

    MPI_Finalize();
    return 0;
}

void setup(int rank, int *num_data, int **data)
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
}

void merge_sort(int **data, int **tmp_data, int left, int right)
{
    if (left < right) {
        int pivot = (left + right) / 2;
        merge_sort(data, tmp_data, left, pivot);
        merge_sort(data, tmp_data, pivot + 1, right);
        merge(data, tmp_data, left, pivot + 1, right);
    }
}

void merge(int **data, int **tmp_data, int left_pos, int right_pos, int right_end)
{
    int left_end = right_pos - 1;
    int tmp_pos = left_pos;
    int num = right_end - left_pos + 1;
    while (left_pos <= left_end && right_pos <= right_end) {
        if (data[left_pos] <= data[right_pos]) {
            tmp_data[tmp_pos++] = data[left_pos++];
        } else {
            tmp_data[tmp_pos++] = data[right_pos++];
        }
    }
    while(left_pos <= left_end) {
        tmp_data[tmp_pos++] = data[left_pos++];
    }
    while(right_pos <= right_end) {
        tmp_data[tmp_pos++] = data[right_pos++];
    }
    for(int i = 0; i < num; i++, right_end--) {
        data[right_end] = tmp_data[right_end];
    }
}
