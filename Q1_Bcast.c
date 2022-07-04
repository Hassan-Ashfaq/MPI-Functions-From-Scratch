#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int rank = -1; 
    int nprocs = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Request request = MPI_REQUEST_NULL;
    MPI_Status status;

    char rdata[] = "";
    if (rank == 0)
    {
        char sdata[] = "Hello PDC";
        for(int itr=1; itr<nprocs; itr++){
            MPI_Isend(&sdata, 9, MPI_CHAR, itr, (itr+1), MPI_COMM_WORLD, &request);
        }
    }else if (rank>0){ 
        MPI_Irecv(&rdata, 9, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
    }
    
    MPI_Wait(&request,&status);
    if(rank>0){
        printf("Process %d Received: %s\n", rank, rdata);
    }

    MPI_Finalize();
    return 0;
}
