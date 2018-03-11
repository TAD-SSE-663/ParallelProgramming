#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define MSG_MAX 100

int main()
{
	char msg[MSG_MAX];
	int num_processes;
	int process_rank;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

	if (process_rank != 0) {
		sprintf(msg, "Hello World! -Love Process %d, a member of the %d Processes.",
			(process_rank + 1), num_processes);
		MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	} else {
		printf("---    MPI Showcase - Hello   ---\n\n");
		printf("Hello World! -Love Process %d, a member of the %d Processes.\n",
			(process_rank + 1), num_processes);
		for (int i = 1; i < num_processes; i++) {
			MPI_Recv(msg, MSG_MAX, MPI_CHAR, i, 0, MPI_COMM_WORLD, 
				MPI_STATUS_IGNORE);
			printf("%s\n", msg);
		}
	}

	MPI_Finalize();
	return 0;
}