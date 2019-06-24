#include <stdio.h>
#include "mpi.h"
#include <time.h>
#include <unistd.h>

#define N 10000 // tamaño arreglo
#define Rango 104729 // rango primos

int primo(int n){  //retorna 0 cuando no es primo y 1 cuando es primo
    int i, res, mitad;
    i=2;
    res = n > 1;
    mitad =n/2;
    while (res!=0 && i<= mitad)
    {
        res= n%i;
        i++;
    }
    return res;
}

double sampleTime() {
 struct timespec tv;
 clock_gettime(CLOCK_MONOTONIC_RAW, &tv);
 return ((double)tv.tv_sec+((double)tv.tv_nsec)/1000000000.0);
}

int main(int argc, char *argv[])		
{
	int rank,i,j,esPrimo;
    int rango[2];
    rango[0]=0;
    rango[1]=0;
    int arreglo[N];
    double tiempo=sampleTime();

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (rank == 0) {
        // if root process we read the value to broadcast
        //printf("Rank %d inicializando buffer\n",rank);
	    rango[0]=0;
        rango[1]=Rango;
    } 
    //else {
    //	printf("process %d: Before MPI_Bcast, value is %d\n", rank, rango[1]); 
	//}

    // each processor calls MPI_Bcast, data is broadcast from root processor and ends up in everyone value variable
    // root process uses MPI_Bcast to broadcast the value, each other process uses MPI_Bcast to receive the broadcast value
    
    //MPI_Barrier(MPI_COMM_WORLD); 
    MPI_Bcast(&rango, 2, MPI_INT, 0, MPI_COMM_WORLD);

    //printf("process %d: After MPI_Bcast, value is %d\n", rank, rango[1]);

    j=rank;
    i=0;
    while (j<rango[1])
    {
        esPrimo=primo(j);
        if (esPrimo==1)
        {
            arreglo[i]=j;
            i++;
            //printf("process %d: encontro primo %d \n", rank,j);
        }
        j+=world_size;
    }
/* 
    for (i = 0; i < world_size; i++)
    {
        MPI_Barrier(MPI_COMM_WORLD); 
        if(rank==i){
            printf("process %d: Arreglo Primos=[", rank);
            for ( j= 0; j < N; j++)
            {
                printf("%d ",arreglo[j]);
                if(arreglo[j]==0){
                    j=N+1;
                }
            }
            printf(" ]\n");
        }
    }*/
    tiempo = sampleTime() - tiempo;
    printf("Soy el proceso número: %d  mi tiempo fue de: %f \n",rank,tiempo);
    MPI_Finalize();

	return 0;
}