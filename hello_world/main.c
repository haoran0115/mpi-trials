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
    printf("proc %s, rank %d out of %d processors\n", name, rank, size);

    // finalized mpi 
    MPI_Finalize();
    
    return 0;
}

