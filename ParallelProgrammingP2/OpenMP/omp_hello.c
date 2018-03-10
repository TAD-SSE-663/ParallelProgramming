#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#ifdef _OPENMP
#	include <omp.h>
#endif

void Hello();

int main(int argc, char* argv[])
{
    printf("---    OpenMP Showcase - Hello    ---\n\n");
    int thread_count = strtol(argv[1], NULL, 10);
#   pragma omp parallel num_threads(thread_count)
    Hello();
    system("pause");
    return 0;
}

void Hello()
{
#ifdef _OPENMP
    int thread_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
#else
    int thread_rank = 0;
    int thread_count = 1;
#endif
    printf("Hello world! -Love Thread %d, a member of the %d Threads.\n", thread_rank  + 1, thread_count);
}