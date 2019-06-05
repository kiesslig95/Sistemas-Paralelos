#include <mpi.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[]) {
    int rank;
    char msg[20];
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        strcpy(msg, "hola");
        MPI_Send(msg, strlen(msg)+1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    }
    else if (rank == 1) {
        MPI_Recv(msg, 20, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        printf("Recibido: %s\n", msg);
    }
    MPI_Finalize();
    return 0;
}