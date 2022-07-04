#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    srand(time(0));
    int n = rand() % 5 + 1;

    int rank = -1; 
    int nprocs = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Request request = MPI_REQUEST_NULL;
    MPI_Request master_request = MPI_REQUEST_NULL;
    MPI_Status status;
    MPI_Status master_status;

    int master_message_size = 0;
    char Meg_T[nprocs];
    char Return_Data[nprocs];
    int CheckPoint = 0;
    if (rank == 0)
    {
        for(int val=1; val<nprocs; val++)
        {
            char rec;
            MPI_Irecv(&rec, 1, MPI_CHAR, val, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
            MPI_Wait(&request,&status);
            Meg_T[val] = rec;
        }
        printf("Total Message Received At Process %d: ", rank);
        for(int arr=1; arr<(nprocs); arr++){
            printf("%c", Meg_T[arr]);
        }
        printf("\n");
    }else if (rank>0){
        int send_size = n;
        char arr[nprocs];
        for(int i=0; i<nprocs; i++){
            arr[i] = 'A'+(i+rank);
        }
        printf("Message in Process %d: ", rank);
        for(int itr=0; itr<nprocs; itr++){
            printf("%c", arr[itr]);
        }
        printf("\n");

        for(int itr=0; itr<nprocs; itr++){
            MPI_Isend(&arr[itr], 1, MPI_CHAR, itr, n, MPI_COMM_WORLD, &request);
        }

        // MPI_Irecv(&Return_Data, 30, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &master_request);
        for(int val=1; val<nprocs; val++)
        {
            char rec;
            MPI_Irecv(&rec, 1, MPI_CHAR, val, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
            MPI_Wait(&request,&status);
            Return_Data[val] = rec;
        }
        
        printf("Total Message Received At Process %d: ", rank);
        for(int arr=1; arr<(nprocs); arr++){
            printf("%c", Return_Data[arr]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
