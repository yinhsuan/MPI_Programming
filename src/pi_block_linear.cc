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

    static unsigned int seed = world_rank;
    long long number_in_circle = 0;
    long long count = tosses / world_size;
    long long remain = tosses % world_size;
    double x, y, distance_squared;
    MPI_Status status;

    // TODO: init MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); // find out process rank
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); // find out numbe of process

    if (world_rank > 0)
    {
        // TODO: handle workers
        for ( int toss = 0; toss < count; toss++) {
            x = (double) rand_r(&seed)/RAND_MAX;
            y = (double) rand_r(&seed)/RAND_MAX;
            distance_squared = x * x + y * y;
            if (distance_squared <= 1) {
                number_in_circle++;
            }
        }
        MPI_Send(&number_in_circle, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    }
    else if (world_rank == 0)
    {
        // TODO: master
        for ( int toss = 0; toss < (count+remain); toss++) {
            x = (double) rand_r(&seed)/RAND_MAX;
            y = (double) rand_r(&seed)/RAND_MAX;
            distance_squared = x * x + y * y;
            if (distance_squared <= 1) {
                number_in_circle++;
            }
        }
    }

    if (world_rank == 0)
    {
        // TODO: process PI result
        long long local_number = 0;
        for (int src=1; src<world_size; src++) {
            MPI_Recv(&local_number, 1, MPI_LONG_LONG, src, 0, MPI_COMM_WORLD, &status);
            number_in_circle += local_number;
        }
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
