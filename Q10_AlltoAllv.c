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

    int CheckPoint = 0;
    if (rank == 0)
    {
        int Master_Check = 0;
        char Meg_T[100];
        int next_start = 0;
        int SIZE = 0;
        for(int val=1; val<nprocs; val++)
        {
            int number_amount;
            MPI_Status status;
            MPI_Probe(val, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_CHAR, &number_amount);
            char *rdata = (char*)malloc(sizeof(char) * number_amount);
            MPI_Irecv(rdata, number_amount, MPI_CHAR, val, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
            MPI_Wait(&request,&status);

            SIZE += number_amount;
            for(int itr=0; itr<number_amount; itr++){
                if(rdata[itr]==0){
                    break;
                }
                Meg_T[Master_Check] = rdata[itr];
                Master_Check++;
            }
        }
        printf("Total Message Received By Process %d: ", rank);
        for(int arr=0; arr<SIZE; arr++){
            printf("%c", Meg_T[arr]);
        }
        printf("\n");
    }else if (rank>0){
        printf("Mesage in Process %d: ", rank);
        for(int sender=0; sender<nprocs; sender++){
            char arr[n];
            int X_s = n;
            for(int i=0; i<n; i++){
                arr[i] = 'A'+i;
                printf("%c", arr[i]);
            }
            if (sender==rank){
                continue;
            }
            MPI_Isend(&arr, X_s, MPI_CHAR, sender, (rank+1), MPI_COMM_WORLD, &request);
            printf("-");
        }
        printf("\n");

        int Master_Check = 0;
        char Return_Data[100];
        int next_start = 0;
        int SIZE = 0;
        for(int val=1; val<nprocs; val++)
        {
            if (val==rank){
                continue;
            }
            int number_amount;
            MPI_Status status;
            MPI_Probe(val, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_CHAR, &number_amount);
            char *rdata = (char*)malloc(sizeof(char) * number_amount);
            MPI_Irecv(rdata, number_amount, MPI_CHAR, val, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
            MPI_Wait(&request,&status);

            SIZE += number_amount;
            for(int itr=0; itr<number_amount; itr++){
                if(rdata[itr]==0){
                    break;
                }
                Return_Data[Master_Check] = rdata[itr];
                Master_Check++;
            }
        }
        printf("Total Message Received By Process %d: ", rank);
        for(int arr=0; arr<SIZE; arr++){
            printf("%c", Return_Data[arr]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
