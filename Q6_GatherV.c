#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    srand(time(0));
    int n = rand() % 10 + 1;

    int rank = -1; 
    int nprocs = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Request request = MPI_REQUEST_NULL;
    MPI_Status status;

    char Meg_T[30] = " ";
    if (rank == 0)
    {
        for(int fill=0; fill<30; fill++){
            Meg_T[fill] = '-';
        }
        
        int next_start = 0;
        for(int val=1; val<nprocs; val++)
        {
            int number_amount;
            MPI_Status status;
            MPI_Probe(val, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_CHAR, &number_amount);
            char *rdata = (char*)malloc(sizeof(char) * number_amount);
            MPI_Irecv(rdata, number_amount, MPI_CHAR, val, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
            MPI_Wait(&request,&status);

            int idx;
            printf("Enter Index No. to Append Process %d\n  Message in Process (Between %d-30): ", val, next_start);
            scanf("%d", &next_start);
            int t = next_start;
            for(int itr=0; itr<number_amount; itr++){
                if(rdata[itr]==0){
                    break;
                }
                Meg_T[t] = rdata[itr];
                t++;
            }
            if(rank==0){
                printf("\nProcess %d Received From %d: %s\n", rank, val, rdata);
                printf("------------------------------\n");
            }
        }
        printf("\nTotal Message Received: ");
        for(int arr=0; arr<(30); arr++){
            printf("%c", Meg_T[arr]);
        }
        printf("\n");
    }else if (rank>0){
        int send_size = n;
        char arr[send_size];
        for(int i=0; i<send_size; i++){
            arr[i] = 'A'+i;
        }
        MPI_Isend(&arr, send_size, MPI_CHAR, 0, n, MPI_COMM_WORLD, &request);
    }

    MPI_Finalize();
    return 0;
}
