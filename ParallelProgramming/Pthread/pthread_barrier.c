#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int thread_count;
int counter;
pthread_mutex_t mutex;
pthread_cond_t condition;

void* Barrier(void* rank);

int main(int argc, char* argv[])
{
    printf("---    Pthreads Showcase - Barrier    ---\n\n");

    // Parallel Execution.
    counter = 0;
    pthread_mutex_init(&mutex, NULL); // Initialize mutex.
    pthread_cond_init(&condition, NULL); // Initializes condition.

	// Get user specified number of threads in base 10.
	thread_count = strtol(argv[1], NULL, 10);

    // Allocate enough memory for each of the threads.
	pthread_t* thread_handles;
	thread_handles = malloc(thread_count * sizeof(pthread_t));

	// Creation of each of the threads. Starts each thread execution.
	for (long thread = 0; thread < thread_count; thread++) {
		pthread_create(&thread_handles[thread], NULL, Barrier, (void*) thread);
    }
	// Stopping of each of the threads.
	for (long thread = 0; thread < thread_count; thread++) {
		pthread_join(thread_handles[thread], NULL);
	}
	free(thread_handles);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);

    system("pause");
    return 0;
}

void* Barrier(void* rank)
{
    long thread_rank = (long) rank;
    pthread_mutex_lock(&mutex);
    counter++;
    if (counter == thread_count) {
        counter = 0;
        pthread_cond_broadcast(&condition);
    } else {
        while (pthread_cond_wait(&condition, &mutex) != 0);
    }
    pthread_mutex_unlock(&mutex);
    // To test that wait was successfull.
    if (counter == 0) {
        printf("Thread %ld waited at the barrier correctly.\n", thread_rank);
    } else {
        printf("\nThread %ld did not wait at the barrier correctly.\n\n", thread_rank);
    }

    return NULL;
}
