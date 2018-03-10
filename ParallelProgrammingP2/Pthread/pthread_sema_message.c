#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MSG_MAX 100 // Maximum length of messages to be sent.

// Global variables.
sem_t *sema;
int thread_count;
char **messages;

// Function to send messages.
void* Send_MSG(void* rank);

int main(int argc, char* argv[])
{
    printf("---    Pthreads Showcase - Semaphore Messages    ---\n\n");

	// Get user specified number of threads in base 10.
	thread_count = strtol(argv[1], NULL, 10);

    // Initialize semaphores for messages.
    sema = malloc(thread_count * sizeof(sem_t*));
    messages = malloc(thread_count * sizeof(char*));
    for (int i = 0; i < thread_count; i++) {
        sem_init(&sema[i], 0, 0);
        messages[i] = NULL;
    }

    // Allocate enough memory for each of the threads.
	pthread_t* thread_handles;
	thread_handles = malloc(thread_count * sizeof(pthread_t));

	// Creation of each of the threads. Starts each thread execution.
	for (long thread = 0; thread < thread_count; thread++) {
		pthread_create(&thread_handles[thread], NULL, Send_MSG, (void*) thread);
    }
	// Stopping of each of the threads.
	for (long thread = 0; thread < thread_count; thread++) {
		pthread_join(thread_handles[thread], NULL);
	}
    // Destroy each semaphore and free allocated memory.
    for (int i = 0; i < thread_count; i++) {
        sem_destroy(&sema[i]);
        free(messages);
    }
	free(thread_handles);
    free(messages);
    free(sema);

    system("pause");
    return 0;
}

void* Send_MSG(void* rank)
{
    long thread_rank = (long) rank;
    long dest = (thread_rank + 1) % thread_count;
    char* msg = malloc(MSG_MAX * sizeof(char));

    sprintf(msg, "This is a message from thread %ld to thread %ld.", thread_rank, dest);
    messages[dest] = msg;
    sem_post(&sema[dest]); // Notify next thread its message is initialized.
    sem_wait(&sema[thread_rank]); // Wait for current message index to be initialized.
    printf("This thread is Thread %ld. Message: %s\n", thread_rank, messages[thread_rank]);

    return NULL;
}