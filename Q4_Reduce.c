#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    srand(time(0));
    int n = rand() % 26 + 1;

    int rank = -1; 
    int nprocs = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Request request = MPI_REQUEST_NULL;
    MPI_Status status;

    if (rank == 0)
    {
        int total_sum = 0;
        for(int val=1; val<nprocs; val++)
        {
            int rec;
            MPI_Irecv(&rec, 1, MPI_INT, val, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
            MPI_Wait(&request,&status);
            total_sum += rec;
            if(rank==0){
                printf("Process %d Received From %d: %d\n", rank, val, rec);
            }
        }
        printf("\nTotal Message Received (Sum): %d\n", total_sum);
    }else if (rank>0){ 
        int buff = n;
        MPI_Isend(&buff, 1, MPI_INT, 0, n, MPI_COMM_WORLD, &request);
    }

    MPI_Finalize();
    return 0;
}
