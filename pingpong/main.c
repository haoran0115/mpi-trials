#include <stdio.h>
#include <mpi.h>

int main(){
    // initialize mpi environment
    MPI_Init(NULL, NULL);

    // get size and rank
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // get the name of the cpu
    char name[MPI_MAX_PROCESSOR_NAME];
    int len;
    MPI_Get_processor_name(name, &len);

    // main program
    int max_pingpong = 10;
    int count = 0;

    // ping pong logic
    while (count < max_pingpong){
        if (rank == count%2){
            count += 1;
            MPI_Send(&count, 1, MPI_INT, (rank+1)%2, 1, MPI_COMM_WORLD);
            printf("Rank %d send count %2d\n", rank, count);
        }
        else {
            MPI_Recv(&count, 1, MPI_INT, (rank+1)%2, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Rank %d recv count %2d\n", rank, count);
        }

    }
    
    // end main program


    // finalized mpi 
    MPI_Finalize();
    
    return 0;
}

