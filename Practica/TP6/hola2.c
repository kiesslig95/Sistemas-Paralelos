#include <mpi.h>

#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank,total_proc,name;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD,&total_proc);
    MPI_Get_processor_name(processor_name,&name)
    printf("Hola mundo de proceso %d de %d en %s :\n",rank,total_proc, processor_name);
    MPI_Finalize();
    return 0;
}