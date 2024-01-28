#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char message[50];
    sprintf(message, "Hello from Process %d!", rank);

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    MPI_Send(message, strlen(message) + 1, MPI_CHAR, next, 0, MPI_COMM_WORLD);
    MPI_Recv(message, sizeof(message), MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Process %d received: %s\n", rank, message);

    MPI_Finalize();
    return 0;
}
