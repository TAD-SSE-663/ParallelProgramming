#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define BILLION 1E9

sem_t sema;
int thread_count;
int num_data;
int par_sum;
int *data;

void* Sum(void* rank);

int main(int argc, char* argv[])
{
    printf("---    Pthreads Showcase - Semaphore Mutex    ---\n\n");
    struct timespec s_t, e_t;
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

    // Serial Execution.
    clock_gettime(CLOCK_MONOTONIC, &s_t);
    int ser_sum = 0;
    for (int i = 0; i < num_data; i++) {
        ser_sum += data[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;

    printf("Serial Sum: %d\n", ser_sum);
    printf("Serial time: %lf\n\n", tot_time);


    // Parallel Execution.
    clock_gettime(CLOCK_MONOTONIC, &s_t);
    par_sum = 0;
    sem_init(&sema, 0, 1);

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
    sem_destroy(&sema);
	free(thread_handles); // Free up the memory allocated to the threads.
    clock_gettime(CLOCK_MONOTONIC, &e_t);
    tot_time = (e_t.tv_sec - s_t.tv_sec) +
        (e_t.tv_nsec - s_t.tv_nsec) / BILLION;

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
    int sum = 0;
    for (int i = start_index; i < start_index + num_loops; i++) {
        sum += data[i];
    }

    sem_wait(&sema);
    par_sum += sum;
    sem_post(&sema);

    return NULL;
}
