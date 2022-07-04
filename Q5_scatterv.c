#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int rank = -1; 
    int nprocs = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Request request = MPI_REQUEST_NULL;
    MPI_Status status;

    char *rdata;
    int Message_Receive_Size = 0;
    if (rank == 0)
    {
        int arr_size = 0;
        printf("Enter Array Size: ");
        scanf("%d", &arr_size);
        char sdata[arr_size];
        for(int val=0; val<arr_size; val++){
            sdata[val] = 'A'+val;
        }

        printf("\nArr: ");
        for(int val=0; val<arr_size; val++){
            printf("%c",sdata[val]);
        }
        printf("\n\n");

        int send_size[nprocs];
        for(int pro=1; pro<nprocs; pro++){
            int no;
            printf("Enter No. of Elements, Send to Process %d: ", pro);
            scanf("%d", &no);
            send_size[pro] = no;
        }
        printf("\n");

        int Next_Start = 0;
        for(int itr=1; itr<nprocs; itr++){
            char SEND_Message[send_size[itr]];
            int t = 0;
            for(int rex=Next_Start; rex<(Next_Start+send_size[itr]); rex++){
                SEND_Message[t] = sdata[rex];
                t++;
            }
            Next_Start += send_size[itr]; 
             
            MPI_Isend(&SEND_Message, send_size[itr], MPI_CHAR, itr, (itr+1), MPI_COMM_WORLD, &request);
        }
    }else if (rank>0){ 
        int number_amount;
        MPI_Status status;
        MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_CHAR, &number_amount);
        Message_Receive_Size = number_amount;
        rdata = (char*)malloc(sizeof(char) * number_amount);
        MPI_Irecv(rdata, number_amount, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
    }
    
    MPI_Wait(&request,&status);
    if(rank>0){
        printf("Process %d Received: ", rank);
        for (int i=0; i<Message_Receive_Size; i++){
            if(rdata[i]==0){
                break;
            }
            printf("%c", rdata[i]);
        }
        printf("\n");
    }
   
    MPI_Finalize();
    return 0;
}
