#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    int rank,i;
    char msg[20];
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int count=atoi(argv[1]);
    for (i = 0; i < count; i++)
    {
         if (rank == i) {
            strcpy(msg, "hola");
            MPI_Send(msg, strlen(msg)+1, MPI_CHAR, i+1, i, MPI_COMM_WORLD);
        }
        else if (rank == i+1) {
            MPI_Recv(msg, 20, MPI_CHAR, i, i, MPI_COMM_WORLD, &status);
            printf("Recibido: %s\n", msg);
        }
    }

    MPI_Finalize();
    return 0;
}   