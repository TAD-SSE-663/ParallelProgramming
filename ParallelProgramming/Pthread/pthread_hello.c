#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#define BILLION 1E9

int thread_count; // The user specified number or threads to use.

void* Hello(void* rank); // The threaded process.
void* Hello_Overhead(void* rank); // A threaded process to find overhead.

int main(int argc, char* argv[])
{
	printf("---    Pthreads Showcase - Hello    ---\n\n");
	struct timespec ser_s_t, ser_e_t, par_s_t, par_e_t, par_os_t, par_oe_t;
    double ser_tot_time, par_tot_time, par_ovr_tot_time;

	// Get user specified number of threads in base 10.
	thread_count = strtol(argv[1], NULL, 10);
	
	// Serial execution.
	clock_gettime(CLOCK_MONOTONIC, &ser_s_t);
	printf("Hello from the main thread.\n");
	for (int i = 0; i < thread_count; i++) {
		printf("Hello from pseudo-thread %d of %d.\n", (i + 1), thread_count);
	}
	clock_gettime(CLOCK_MONOTONIC, &ser_e_t);
	ser_tot_time = (ser_e_t.tv_sec - ser_s_t.tv_sec) +
		(ser_e_t.tv_nsec - ser_s_t.tv_nsec) / BILLION;

	// Parallel Execution.
    clock_gettime(CLOCK_MONOTONIC, &par_s_t);

	// Allocate enough memory for each of the threads.
	pthread_t* thread_handles;
	thread_handles = malloc(thread_count * sizeof(pthread_t));

	// Creation of each of the threads. Starts each thread execution.
	for (long thread = 0; thread < thread_count; thread++) {
		pthread_create(&thread_handles[thread], NULL, Hello, (void*) thread);
	}
	printf("Hello from the main thread.\n");
	// Stopping of each of the threads.
	for (long thread = 0; thread < thread_count; thread++) {
		pthread_join(thread_handles[thread], NULL);
	}
	free(thread_handles); // Free up the memory allocated to the threads.
    clock_gettime(CLOCK_MONOTONIC, &par_e_t);
    par_tot_time = (par_e_t.tv_sec - par_s_t.tv_sec) +
        (par_e_t.tv_nsec - par_s_t.tv_nsec) / BILLION;


	// Parallel Overhead Execution.
    clock_gettime(CLOCK_MONOTONIC, &par_os_t);

	// Allocate enough memory for each of the threads.
	pthread_t* overhead_thread_handles;
	overhead_thread_handles = malloc(thread_count * sizeof(pthread_t));

	// Creation of each of the threads. Starts each thread execution.
	for (long thread = 0; thread < thread_count; thread++) {
		pthread_create(&overhead_thread_handles[thread], NULL, Hello_Overhead, (void*) thread);
	}
	printf("Hello from the main thread.\n");
	// Stopping of each of the threads.
	for (long thread = 0; thread < thread_count; thread++) {
		pthread_join(overhead_thread_handles[thread], NULL);
	}
	// Free up the memory allocated to the threads.
	free(overhead_thread_handles);
    clock_gettime(CLOCK_MONOTONIC, &par_oe_t);
    par_ovr_tot_time = (par_oe_t.tv_sec - par_os_t.tv_sec) +
        (par_oe_t.tv_nsec - par_os_t.tv_nsec) / BILLION;

    printf("\nTotal elapsed serial time: %f.\n", ser_tot_time);
    printf("Total elapsed parallel time: %f.\n", par_tot_time);
    printf("Total elapsed parallel overhead time: %f.\n\n", par_ovr_tot_time);


	system("pause");
	return 0;
}

void* Hello(void* rank)
{
	long my_rank = (long) rank; // Getting this thread number.

	printf("Hello from thread %ld of %d.\n", (my_rank + 1), thread_count);
	
	return NULL;
}

void* Hello_Overhead(void* rank)
{
	return NULL;
}
