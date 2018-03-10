#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

pthread_rwlock_t rw_lock;
int thread_count;
int num_data;

struct list_node
{
    int data;
    struct list_node *next;
};

struct list_node *root;

int Insert(int val, struct list_node **root_n);
int Delete(int val, struct list_node ** root_n);
void Print(struct list_node *root_n);
void* Test_List(void* rank);

int main(int argc, char* argv[])
{
    printf("---    Pthreads Showcase - Read Write Locks    ---\n\n");

    // Get file information.
    FILE *data_file;
    data_file = fopen(argv[1], "r");
    num_data = strtol(argv[2], NULL, 10);
    char line[10];
    int line_index = 0;
    while (line_index < num_data && fgets(line, sizeof(line), data_file) != NULL)
    {
        Insert(strtol(line, NULL, 10), &root);
        line_index++;
    }

    printf("Initial list:\n\n");
    Print(root);

    // Parallel Execution.
    pthread_rwlock_init(&rw_lock, NULL); // Initialize mutex.

	// Get user specified number of threads in base 10.
	thread_count = strtol(argv[3], NULL, 10);

    // Allocate enough memory for each of the threads.
	pthread_t* thread_handles;
	thread_handles = malloc(thread_count * sizeof(pthread_t));

	// Creation of each of the threads. Starts each thread execution.
	for (long thread = 0; thread < thread_count; thread++)
	{
		pthread_create(&thread_handles[thread], NULL, Test_List, (void*) thread);
    }
	// Stopping of each of the threads.
	for (long thread = 0; thread < thread_count; thread++)
	{
		pthread_join(thread_handles[thread], NULL);
	}
    pthread_rwlock_destroy(&rw_lock); // Destory mutex.
	free(thread_handles); // Free up the memory allocated to the threads.

    printf("\n\nFinal List:\n\n");
    Print(root);

    system("pause");
    return 0;
}

int Insert(int val, struct list_node **root_n)
{
    struct list_node *curr_n = *root_n;
    struct list_node *pred_n = NULL;
    struct list_node *temp_n;
    while (curr_n != NULL && curr_n -> data < val)
    {
        pred_n = curr_n;
        curr_n = curr_n -> next;
    }
    if (curr_n == NULL || curr_n -> data > val)
    {
        temp_n = malloc(sizeof(struct list_node));
        temp_n -> data = val;
        temp_n -> next = curr_n;
        if (pred_n == NULL)
        {
            *root_n = temp_n;
        }
        else
        {
            pred_n -> next = temp_n;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

int Delete(int val, struct list_node **root_n)
{
    struct list_node *curr_n = *root_n;
    struct list_node *pred_n = NULL;
    while (curr_n != NULL && curr_n -> data < val)
    {
        pred_n = curr_n;
        curr_n = curr_n -> next;
    }
    if (curr_n != NULL && curr_n -> data == val)
    {
        if (pred_n == NULL)
        {
            *root_n = curr_n -> next;
            free(curr_n);
        }
        else
        {
            pred_n -> next = curr_n -> next;
            free(curr_n);
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

void Print(struct list_node *root)
{
    struct list_node *temp = root;
    while (temp != NULL)
    {
        printf("%d\n", temp -> data);
        temp = temp -> next;
    }
}

void* Test_List(void* rank)
{
    long thread_rank = (long) rank;
    if ((thread_rank % 10) == 0)
    {
        pthread_rwlock_wrlock(&rw_lock);
        Delete((int)thread_rank, &root);
        pthread_rwlock_unlock(&rw_lock);
    }
    else if ((thread_rank % 5) == 0)
    {
        pthread_rwlock_wrlock(&rw_lock);
        Insert((-1) * (int)thread_rank, &root);
        pthread_rwlock_unlock(&rw_lock);
    }

    return NULL;
}