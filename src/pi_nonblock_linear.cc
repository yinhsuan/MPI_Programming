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

    static unsigned int seed = world_rank;
    long long int number_in_circle = 0;
    long long int count = tosses / world_size;
    long long int remain = tosses % world_size;
    long long int local_number = 0;
    double x, y;
    // MPI_Status status;

    count = (world_rank == 0) ? (count + remain) : count;
    for (int toss = 0; toss<count; toss++) {
        x = (double) rand_r(&seed)/RAND_MAX;
        y = (double) rand_r(&seed)/RAND_MAX;
        if (x * x + y * y <= 1) {
            local_number++;
        }
    }

    if (world_rank > 0)
    {
        // TODO: MPI workers
        MPI_Request request;
        MPI_Isend(&local_number, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD, &request);
    }
    else if (world_rank == 0)
    {
        // TODO: non-blocking MPI communication.
        // Use MPI_Irecv, MPI_Wait or MPI_Waitall.
        MPI_Request requests[world_size - 1];
        MPI_Status statuses[world_size - 1];
        long long int buf;
        for (int src=1; src<world_size; src++) {
            MPI_Irecv(&buf, 1, MPI_LONG_LONG, src, 0, MPI_COMM_WORLD, &requests[src-1]);
            local_number += buf;
        }
        MPI_Waitall(world_size - 1, requests, statuses);
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
