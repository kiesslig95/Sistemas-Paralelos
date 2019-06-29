#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <emmintrin.h>

#define N 10000 // tamaño arreglo
//#define FRango 10000 // fin rango primos
//#define IRango 1 //incio rango primos

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
       4) Rango aux final 
    */
    
    /* arreglo donde guardo los primos encontrados uno por proceso */
    /* se inicializa con un tamaño superior al necesario */
    //int *arreglo_enviar=(int*)malloc(N*sizeof(int)); //deberia corregir esto

    //sendbuf = (int*)malloc(100 * size * sizeof(int))
    double tiempo=sampleTime();
  
    /* %%%%%%%%%%%%%%%%%%%%%%%%%%% MPI %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5 */
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    

    if (rank == 0) {
        /* cargo los rangos */
        int IRango =atoi(argv[1]);
        int FRango =atoi(argv[2]);
        //int bloque =atoi(argv)
 
        /* para trabajar con bloques */
        int cantElem=(FRango - IRango)+1;
        /* tamaño bloques */ 
        int bloque=(cantElem/100);
        /* cantidad de bloques */ 
        int cantBloques= cantElem/bloque;
        /* rango auxiliar numero igual o menos a N*/ 
        int rangoAux=0;

        //printf("Rank %d con bloque %d y cant bloques %d inicializando buffer\n",rank,bloque,cantBloques);
        if(cantElem%bloque!=0){
            rangoAux=(cantBloques*bloque);
        }else
        {
            rangoAux=FRango;
        }

        /* inicio rango */
	    datos[0]=IRango;
        /* fin rango */
        datos[1]=FRango;
        /* tamaño bloque */
        datos[2]=bloque;
        /* rango fin aux */
        datos[3]=rangoAux;
    } 
    int *arreglo_enviar=(int*)malloc(N*sizeof(int));

    /* %%%%%%%%%%%%%%%%%%%%%5 Broadcast %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
    MPI_Bcast(&datos, 4, MPI_INT, 0, MPI_COMM_WORLD);
    
    /* posicion de incio de los rank para recorrer */
    j=datos[2]*rank;

    /* posicion dentro de arreglo_enviar para carga de datos */
    int cantPrimos=0; //la uso para saber cuantos primos encontro el proceso

    /* auxiliar */
    int aux;

    /* cada proceso recorre bloque/s dentro del rangoAux */
    while (j<datos[3])
    {
        int count=j+datos[2];
        for (aux = j; aux < count; aux++)
        {
            esPrimo=primo(aux);
            if (esPrimo==1)
            {
                arreglo_enviar[cantPrimos]=aux;
                cantPrimos++;
                if (rank==0){
                    printf("encontre primo %d =%d \n",cantPrimos, aux);
                }
                
            }
        }
        j+=world_size*datos[2];
    }
    
    /* si la cantidad de elementos del rango no era multiplo del tamaño del bloque reviso ultimos elementos */
    if (datos[1]!=datos[3])
    {
        j=datos[3]+rank;
        while (j<datos[1])
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
    printf("proceso %d encontro %d primos \n",rank,cantPrimos);
    /* redefino reserva de memoria */
    //arreglo_enviar =(int*) realloc(arreglo_enviar, cantPrimos);

    /* recvcounts
	Almacena la cantidad que va a mandar cada proceso: [1,2,3,4,...,n+1]
	Debe reemplazarse por un gather que recolecte un entero 'cant' por cada worker */
    int *arreglo_cantidades;
    //printf("size %d \n", world_size);
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

    MPI_Gatherv(arreglo_enviar,cantPrimos,MPI_INT,resultado,arreglo_cantidades,arreglo_desplazamientos,MPI_INT,0,MPI_COMM_WORLD);

	/* Solo el proceso root debe imprimir el resultado obtenido
	El IF puede omitirse ya que para los demas procesos cant_total==0 y el FOR
	no realizaría ninguna iteración */
	if(rank == 0){
		printf("Cantidad de procesos: %d\n",world_size);
		printf("Cantidad de enteros recibidos con gatherv: %d\n", cant_total);
		printf("Arreglo recibido: [");
		for (i = 0; i < cant_total-1 ; i++)
		{
			printf("%d,",resultado[i]);
		}
		printf("%d]\n",resultado[i]); //Para evitar la ultima coma en la impresion
	}

    //printf("cant Total %d \n",cant_total);
    

    /* %%%%%%%%%%%%%%%%%%%%%% Print primos por procesador %%%%%%%%%%%%%%%%%%%%%%%%% */
    
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
    }
    


    
    
    tiempo = sampleTime() - tiempo;
    printf("Soy el proceso número: %d  mi tiempo fue de: %f \n",rank,tiempo);


    MPI_Finalize();

	return 0;
}