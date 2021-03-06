#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <emmintrin.h>

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%5 Metodo para calcular primos %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
int primo(int n){  
    int i, res, mitad;
    i=2;
    res = n > 1;
    mitad =n/2;
    while (res!=0 && i<= mitad)
    {
        res= n%i;
        i++;
    }
    return res; //retorna 0 cuando no es primo y 1 cuando es primo
}
/* %%%%%%%%%%%%%%%%%% Metodo para calcular los tiempos de ejecucion de los procesos %%%%%%%%%%% */
double sampleTime() {
 struct timespec tv;
 clock_gettime(CLOCK_MONOTONIC_RAW, &tv);
 return ((double)tv.tv_sec+((double)tv.tv_nsec)/1000000000.0);
}

/* %%%%%%%%%%%%%%%%%%%%% Inicio main %%%%%%%%%%%%%%%%%%%% */
int main(int argc, char *argv[])		
{
    int IRango,FRango,bloque,rangoAux;

	int rank,i,j,esPrimo,world_size;
    /*
        rank      : ranking procesador 
        world_size: cantidad de procesadores 
    */

    /* datos que se envian a todos los procesos  */
    int datos[3];
    /* 1) IRango
       2) FRango
       3) tamaño bloque 
    */
    
    double tiempo=sampleTime();
  
    /* %%%%%%%%%%%%%%%%%%%%%%%%%%% MPI %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5 */
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    /* %%%%%%%%%%%%%%%%%%% Solo proceso Root  %%%%%%%%%%%%%%%%%%%%%%%*/
    if (rank == 0) {
        /* cargo los rangos */
        IRango =atoi(argv[1]);
        FRango =atoi(argv[2]);

        /* tamaño bloques */ 
        bloque =atoi(argv[3]);
 
        /* inicio rango */
	    datos[0]=IRango;
        /* fin rango */
        datos[1]=FRango;
        /* tamaño bloque */
        datos[2]=bloque;

    }
    

    /* %%%%%%%%%%%%%%%%%%%%%5 Broadcast %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
    MPI_Bcast(&datos, 3, MPI_INT, 0, MPI_COMM_WORLD);

    /* %%%% cargo valores del Broadcast %%%%%% */
    IRango= datos[0];
    FRango= datos[1];
    bloque= datos[2];

    /* calculo rango auxiliar  */
    /* cantidad de elementos en el rango dado */
    int cantElem=(FRango - IRango)+1;
    /* cantidad de bloques */ 
    int cantBloques= cantElem/bloque;

    /* rango auxiliar numero igual o menos a N*/ 
    if(cantElem%bloque!=0){
        rangoAux=(cantBloques*bloque);
    }else
    {
        rangoAux=FRango;
    }

    /* calculo un tamaño para el arreglo que contendra los primos en cada proceso */
    /* Si la carga esta muy desbalanceda falla */
    int N=cantElem/2;
    int *arreglo_enviar=(int*)malloc(N*sizeof(int));
    
    /* posicion de incio de los rank para recorrer */
    j=IRango+bloque*rank;

    /* posicion dentro de arreglo_enviar para carga de datos */
    int cantPrimos=0; //la uso para saber cuantos primos encontro el proceso

    /* auxiliar */
    int aux;

    /* cada proceso recorre bloque/s dentro del rangoAux */
    if(bloque!=1){
        while (j<rangoAux)
        {
            int count=j+bloque;
            for (aux = j; aux < count; aux++)
            {
                esPrimo=primo(aux);
                if (esPrimo==1)
                {
                    arreglo_enviar[cantPrimos]=aux;
                    cantPrimos++;
                }
            }
            j+=world_size*bloque;
        }
    }else
    {
        //printf("Bloque tamaño 1 \n");
        rangoAux=0;
    }
    
    /* si la cantidad de elementos del rango no era multiplo del tamaño del bloque reviso ultimos elementos */
    if (FRango!=rangoAux)
    {
        j=rangoAux+rank;
        while (j<=FRango)
        {
            esPrimo=primo(j);
            if (esPrimo==1)
            {
                arreglo_enviar[cantPrimos]=j;
                cantPrimos++;
            }
            j+=world_size;
        }
    }
    /* lo coloque aca porque los gather son bloqueantes */
    tiempo = sampleTime() - tiempo;
    printf("Soy el proceso número: %d termine mi calculo de primos y mi tiempo fue de: %f \n",rank,tiempo);

    /* se usa para enviar las candidades de primos que encontro cada proceso al master */
    int *arreglo_cantidades;
    /*  MPI_Gather  */
    /*Operación bloqueante que recolecta, en un proceso, datos de todos los procesos (inclusive
    los que tiene él mismo) asociados a un comunicador. La recolección de segmentos de datos
    es ordenada por número de rank. */

    if(rank==0) arreglo_cantidades= (int *) malloc(world_size*sizeof(int));
    MPI_Gather(&cantPrimos,1, MPI_INT, arreglo_cantidades,1, MPI_INT, 0, MPI_COMM_WORLD);

    aux = 0;
	int *arreglo_desplazamientos = (int *) malloc(world_size*sizeof(int));
 	for (i = 0; i<world_size; i++){
		arreglo_desplazamientos[i]=aux;
		aux += arreglo_cantidades[i];
	}

    int cant_total = 0;
	int *resultado;
	if(rank == 0){
	 	for (i = 0; i<world_size; i++){
			cant_total += arreglo_cantidades[i];
		}
		resultado = (int *) malloc(cant_total*sizeof(int));
	}

    

    /* envio los arreglos con los primos al master */
    MPI_Gatherv(arreglo_enviar,cantPrimos,MPI_INT,resultado,arreglo_cantidades,arreglo_desplazamientos,MPI_INT,0,MPI_COMM_WORLD);
    
	if(rank == 0){
		printf("Cantidad de procesos: %d\n",world_size);
        printf("Tamaño del bloque: %d\n",bloque);
		printf("Cantidad de enteros recibidos con gatherv: %d\n", cant_total);
        /* 
		printf("Arreglo recibido: [");
		for (i = 0; i < cant_total-1 ; i++)
		{
			printf("%d,",resultado[i]);
		}
		printf("%d]\n",resultado[i]); */
    }

    //printf("cant Total %d \n",cant_total);
    

    /* %%%%%%%%%%%%%%%%%%%%%% Print primos por procesador %%%%%%%%%%%%%%%%%%%%%%%%% */
    /* 
    for (i = 0; i < world_size; i++)
    {
        MPI_Barrier(MPI_COMM_WORLD); 
        if(rank==i){
            printf("process %d: Arreglo Primos=[", rank);
            for ( j= 0; j < cantPrimos; j++)
            {
                printf("%d ",arreglo_enviar[j]);
            }
            printf(" ]\n");
    }
    }*/
    


    
    
    //tiempo = sampleTime() - tiempo;
    //printf("Soy el proceso número: %d  mi tiempo fue de: %f \n",rank,tiempo);


    MPI_Finalize();


	return 0;
}