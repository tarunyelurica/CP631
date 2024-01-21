#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

double estimate_pi_parallel(long long num_samples) {
    int rank, size;
    long long inside_circle = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank); 
    for (long long i = 0; i < num_samples; ++i) {
        double x = ((double)rand() / RAND_MAX);
        double y = ((double)rand() / RAND_MAX);

        double distance = x * x + y * y;

        if (distance <= 1) {
            inside_circle++;
        }
    }

    if (rank != 0) {
        MPI_Send(&inside_circle, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        long long total_inside_circle = inside_circle;
        for (int i = 1; i < size; ++i) {
            MPI_Recv(&inside_circle, 1, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_inside_circle += inside_circle;
        }

        double pi_estimate = (total_inside_circle / ((double)num_samples * size)) * 4;
        return pi_estimate;
    } else {
        return 0.0;
    }
}

int main() {
    MPI_Init(NULL, NULL);

    long long num_samples = 1000000; 
    double pi_parallel = estimate_pi_parallel(num_samples);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        printf("Parallel estimate of Pi: %lf\n", pi_parallel);
    }

    MPI_Finalize();

    return 0;
}
