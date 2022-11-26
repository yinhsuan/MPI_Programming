#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    // ---

    // TODO: MPI init
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int window = 1;
    long long int local_number = 0;
    long long int number_in_circle = 0;
    long long int count = tosses / world_size;
    long long int remain = tosses % world_size;
    static unsigned int seed = world_rank;
    double x, y;
    MPI_Status status;

    // TODO: binary tree redunction
    count = (world_rank == 0) ? (count + remain) : count;
    for (int toss = 0; toss<count; toss++) {
        x = (double) rand_r(&seed)/RAND_MAX;
        y = (double) rand_r(&seed)/RAND_MAX;
        if (x * x + y * y <= 1) {
            local_number++;
        }
    }

    while (window < world_size) {
        window *= 2;
        MPI_Barrier(MPI_COMM_WORLD);

        // handle send
        if (world_rank % window == (window/2)) {
            int dst = world_rank - (window/2);
            MPI_Send(&local_number, 1, MPI_LONG_LONG, dst, 0, MPI_COMM_WORLD);
        }
        // handle receive 
        else if (world_rank % window == 0) {
            long long int buf = 0;
            int src = world_rank + (window/2);
            MPI_Recv(&buf, 1, MPI_LONG_LONG, src, 0, MPI_COMM_WORLD, &status);
            local_number += buf;
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }

    if (world_rank == 0)
    {
        // TODO: PI result
        number_in_circle = local_number;
        pi_result = 4 * number_in_circle /((double) tosses);


        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}
