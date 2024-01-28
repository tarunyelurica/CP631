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

    MPI_Request send_request, recv_request;
    MPI_Status status;

    MPI_Isend(message, strlen(message) + 1, MPI_CHAR, next, 0, MPI_COMM_WORLD, &send_request);
    MPI_Irecv(message, sizeof(message), MPI_CHAR, prev, 0, MPI_COMM_WORLD, &recv_request);

    MPI_Wait(&send_request, &status);
    MPI_Wait(&recv_request, &status);

    printf("Process %d received: %s\n", rank, message);

    MPI_Finalize();
    return 0;
}
