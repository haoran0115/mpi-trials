#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int dot_product(int *A, int *B, int M, int N, int P, int i, int j){
    // compute sum_k A_ik * B_kj
    // A_ik = A[i*N + k]
    // B_kj = B[k*P + j]
    int ret = 0;
    for (int k = 0; k < N; k++) {
        ret += A[i*N + k] * B[k*P + j];
    }
    return ret;
}

// matrix multiplication
int main(){
    // allocate matrices
    int N = 20;
    int* A = (int *)malloc(sizeof(int) * N * N);
    int* B = (int *)malloc(sizeof(int) * N * N);
    int* C = (int *)malloc(sizeof(int) * N * N);
    // initialize data
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i*N + j] = 1;
            B[i*N + j] = 2;
        }
    }

    // mpi initialization
    MPI_Init(NULL, NULL);
    // fetch size and rank
    int rank, size;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // exit if only 1 threads
    if (size < 2){
        printf("No enough workers! Program exit.\n");
        return 1;
    }

    // main program: compute C = A * B;
    // set parallel parameters
    int num_jobs = N / size;
    int remainder = N % size;

    // master task
    if (rank == 0){
        // send data
        int* A_ptr = A;
        int* B_ptr = B;
        for (int i = 1; i < size; i++){
            MPI_Send(A_ptr, N*num_jobs, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(B_ptr,        N*N, MPI_INT, i, 1, MPI_COMM_WORLD);
            A_ptr += num_jobs * N;
        }

        // tail case
        int start_idx = num_jobs * (size-1);
        int end_idx   = N;
        printf("rank %2d start %2d end %d\n", rank, start_idx, end_idx);
        // computation
        for (int i = start_idx; i < end_idx; i++){
            for (int j = 0; j < N; j++){
                C[i*N + j] = dot_product(A, B, N, N, N, i, j);
            }
        }
    }
    // slave task
    else {
        // allocation 
        int *a;
        int *B;
        int *c;
        a = (int *)malloc(sizeof(int) * N * num_jobs);
        c = (int *)malloc(sizeof(int) * N * num_jobs);
        B = (int *)malloc(sizeof(int) * N * N);
        
        // receive data from master
        MPI_Recv(a, N*num_jobs, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(B,        N*N, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // normal case
        int start_idx = (rank - 1) * num_jobs;
        int end_idx   = start_idx + num_jobs;
        // computation
        for (int i = 0; i < num_jobs; i++){
            for (int j = 0; j < N; j++){
                c[i*N + j] = dot_product(a, B, N, N, N, i, j);
            }
        }
        printf("rank %2d start %2d end %d c[0] %d\n", rank, start_idx, end_idx, c[0]);

        // send results
        MPI_Send(c, N*num_jobs, MPI_INT, 0, 2, MPI_COMM_WORLD);        
        printf("rank %2d finish sending\n", rank);
    }

    // receive data
    if (rank==0) {
        // receive results
        printf("Enter receive\n");
        int *C_ptr = C;
        for (int i = 1; i < size; i++){
            MPI_Recv(C_ptr, N*num_jobs, MPI_INT, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            C_ptr += num_jobs * N;
        }
    }


    // mpi finalize
    MPI_Finalize();


    // free matrices
    free(A);
    free(B);
    free(C);

    return 0;
}



