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

    int rdata = 0;
    if (rank == 0)
    {
        int sdata[nprocs];
        for(int val=0; val<nprocs; val++){
            sdata[val] = (val+10)*2;
        }
        for(int itr=1; itr<nprocs; itr++){
	        // printf("%d : %d\n", itr, sdata[itr-1]); 
            MPI_Isend(&sdata[(itr-1)], 1, MPI_INT, itr, (itr+1), MPI_COMM_WORLD, &request);
        }
    }else if (rank>0){ 
        int buf = 0;
        MPI_Irecv(&rdata, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
    }
    
    MPI_Wait(&request,&status);
    if(rank>0){
        printf("Process %d Received: %d\n", rank, rdata);
    }
   
    MPI_Finalize();
    return 0;
}
