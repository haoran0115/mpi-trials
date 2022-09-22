#include <stdio.h>
#include <mpi.h>

int main(){
    // mpi initialize
    MPI_Init(NULL, NULL);
    // find rank and size
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int num = 0;
    int arr[2] = {0, 0};
    if (rank==0) {
        num = 10;
        MPI_Send(&num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } 
    else if (rank==1) {
        arr[0] = 99;
        // send data to 2
        MPI_Send(arr, 2, MPI_INT, 2, 0, MPI_COMM_WORLD);
        // recieve data from 0
        MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if (rank==2){
        // recieve data from 1
        MPI_Recv(arr, 2, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    printf("rank %2d num %2d arr0 %2d arr1 %2d\n", rank, num, arr[0], arr[1]);

    // mpi finalize
    MPI_Finalize();

    return 0;
}

