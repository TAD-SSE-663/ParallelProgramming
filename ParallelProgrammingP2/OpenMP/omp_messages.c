#include <stdlib.h>
#include <stdio.h>
#ifdef _OPENMP
#	include <omp.h>
#endif

#define MSG_MAX 100

int main(int argc, char* argv[])
{
    printf("---    OpenMP Showcase - Messages    ---\n\n");
    
    int thread_count = strtol(argv[1], NULL, 10);
    char** messages = malloc(thread_count * sizeof(char*));
    int dest;
    int curr_thread;
    char* msg;
#   pragma omp parallel num_threads(thread_count) default(none) \
        private(dest, curr_thread, msg) shared(messages, thread_count)
    {
        curr_thread = omp_get_thread_num();
        dest = (curr_thread + 1) % thread_count;
        msg = malloc(MSG_MAX * sizeof(char));
        sprintf(msg, "This is a message from %d to %d.", 
            curr_thread, dest);
        messages[dest] = msg;
#       pragma omp barrier
        printf("This thread is thread %d. Message: %s\n",
            curr_thread, messages[curr_thread]);
    }

    system("pause");
    return 0;
}
