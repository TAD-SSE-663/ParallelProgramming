#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#define BILLION 1E9

int thread_count;
int num_data;
int par_sum;
int flag;
int *data;

void* Sum(void* rank);

int main(int argc, char* argv[])
{
    printf("---    Pthreads Showcase - Busy Wait    ---\n\n");
    struct timespec start_time, end_time;
    double tot_time;

    // Get file information.
    FILE *data_file;
    data_file = fopen(argv[1], "r");
    num_data = strtol(argv[2], NULL, 10);
    char line[10];
    data = malloc(sizeof(int) * num_data);
    int line_index = 0;
    while (line_index < num_data && fgets(line, sizeof(line), data_file) != NULL) {
        data[line_index++] = strtol(line, NULL, 10);
    }

    clock_gettime(CLOCK_MONOTONIC, &start_time);
    // Serial Execution.
    int ser_sum = 0;
    for (int i = 0; i < num_data; i++) {
        ser_sum += data[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    tot_time = (end_time.tv_sec - start_time.tv_sec) +
        (end_time.tv_nsec - start_time.tv_nsec) / BILLION;

    printf("Serial Sum: %d\n", ser_sum);
    printf("Serial time: %lf\n\n", tot_time);


    // Parallel Execution.
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    par_sum = 0;
    flag = 0;

	// Get user specified number of threads in base 10.
	thread_count = strtol(argv[3], NULL, 10);

    // Allocate enough memory for each of the threads.
	pthread_t* thread_handles;
	thread_handles = malloc(thread_count * sizeof(pthread_t));

	// Creation of each of the threads. Starts each thread execution.
	for (long thread = 0; thread < thread_count; thread++) {
		pthread_create(&thread_handles[thread], NULL, Sum, (void*) thread);
    }
	// Stopping of each of the threads.
	for (long thread = 0; thread < thread_count; thread++) {
		pthread_join(thread_handles[thread], NULL);
	}
	free(thread_handles); // Free up the memory allocated to the threads.
    
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    tot_time = (end_time.tv_sec - start_time.tv_sec) +
        (end_time.tv_nsec - start_time.tv_nsec) / BILLION;

    printf("Parallel Sum: %d\n", par_sum);
    printf("Parallel time: %lf\n\n", tot_time);

    system("pause");
    return 0;
}

void* Sum(void* rank)
{
    long thread_rank = (long) rank;
    int num_loops = num_data / thread_count;
    int start_index = num_loops * thread_rank;

    // Private sum to be done in parallel.
    int sum = 0;
    for (int i = start_index; i < start_index + num_loops; i++) {
        sum += data[i];
    }

    while (flag != thread_rank); // Wait for flag to specify this thread.
    par_sum += sum; // Perform critical section execution.
    flag = (flag + 1) % thread_count; // Increment flag.

    return NULL;
}
