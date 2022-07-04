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
    MPI_Request master_request = MPI_REQUEST_NULL;
    MPI_Status status;
    MPI_Status master_status;

    char Meg_T[nprocs];
    char Return_Data[nprocs];
    if (rank == 0)
    {
        for(int val=1; val<nprocs; val++)
        {
            char rec;
            MPI_Irecv(&rec, 1, MPI_CHAR, val, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
            MPI_Wait(&request,&status);
            Meg_T[val] = rec;
            if(rank==0){
                printf("Process %d Received From %d: %c\n", rank, val, rec);
            }
        }
        printf("\nTotal Message Received: ");
        for(int arr=1; arr<(nprocs); arr++){
            printf("%c", Meg_T[arr]);
        }
        printf("\n");
        printf("\n======================================================\n\n");

        printf("Sending To All Process of Size: %d\n", nprocs);
        for(int val=1; val<nprocs; val++)
        {
            MPI_Isend(&Meg_T, nprocs, MPI_CHAR, val, n, MPI_COMM_WORLD, &master_request);
        }

    }else if (rank>0){ 
        char buff = 'A'+n;
        MPI_Isend(&buff, 1, MPI_CHAR, 0, n, MPI_COMM_WORLD, &request);
        MPI_Irecv(&Return_Data, nprocs, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &master_request);
    }

    MPI_Wait(&master_request, &master_status);
    if(rank>0){
        printf("Message Received By Process %d: ", rank);
        for(int arr=1; arr<nprocs; arr++){
            printf("%c", Return_Data[arr]);
        }
        printf("\n");
    }
    MPI_Finalize();
    return 0;
}
