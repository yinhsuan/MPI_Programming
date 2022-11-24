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

    // TODO: init MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); // find out process rank
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); // find out numbe of process

    static unsigned int seed = world_rank;
    long long int number_in_circle = 0;
    long long int count = tosses / world_size;
    long long int remain = tosses % world_size;
    long long int local_number = 0;
    double x, y, distance_squared;
    MPI_Status status;

    for (int toss = 0; toss<count; toss++) {
        x = (double) rand_r(&seed)/RAND_MAX;
        y = (double) rand_r(&seed)/RAND_MAX;
        if (x * x + y * y <= 1) {
            local_number++;
        }
    }

    if (world_rank > 0)
    {
        // TODO: handle workers
        MPI_Send(&local_number, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    }
    else if (world_rank == 0)
    {
        // TODO: master
        number_in_circle = local_number;
        long long int buf = 0;
        for (int worker=1; worker<world_size; worker++) {
            MPI_Recv(&buf, 1, MPI_LONG_LONG, worker, 0, MPI_COMM_WORLD, &status);
            number_in_circle += buf;
        }
    }

    if (world_rank == 0)
    {
        // TODO: process PI result
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