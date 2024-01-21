
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char* argv[]) {
    int my_rank;    
    int p;         
    int source;    
    int dest;      
    int tag = 0;   
    char* message;  
    MPI_Status status; 
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    if (my_rank != 0) {
        asprintf(&message, "Greetings from process %d!", my_rank+1);
        dest = (my_rank + 1) % p; 
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
        printf("Process %d sent message: %s\n", my_rank, message);
        free(message);  
    } else { 
        source = p - 1; 
        MPI_Probe(source, tag, MPI_COMM_WORLD, &status);
        int count;
        MPI_Get_count(&status, MPI_CHAR, &count);
        message = (char*)malloc(count * sizeof(char));

        MPI_Recv(message, count, MPI_CHAR, status.MPI_SOURCE, tag, MPI_COMM_WORLD, &status);
        printf("Process 0 received message: %s\n", message);
        free(message); 
    }
    MPI_Finalize();
    return 0;
}
