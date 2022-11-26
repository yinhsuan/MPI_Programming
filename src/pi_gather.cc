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
    long long int *recvbuf;
    double x, y;

    count = (world_rank == 0) ? (count + remain) : count;
    for (int toss = 0; toss<count; toss++) {
        x = (double) rand_r(&seed)/RAND_MAX;
        y = (double) rand_r(&seed)/RAND_MAX;
        if (x * x + y * y <= 1) {
            local_number++;
        }
    }

    // TODO: use MPI_Gather
    if (world_rank == 0) {
        recvbuf = (long long int *) malloc(world_size * sizeof(long long int));
    }
    MPI_Gather(&local_number, 1, MPI_LONG_LONG, recvbuf, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        // TODO: PI result
        for (int i=1; i<world_size; i++) {
            local_number += recvbuf[i];
        }
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
