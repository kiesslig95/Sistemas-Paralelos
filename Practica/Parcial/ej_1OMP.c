#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

//mejoras posibles trabajar sobre arreglos difentes enn cada hilo, por lo que puedo quitar la zona critica 
//de escritura en arreglo

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

int main(int argc, char *argv[]){
    int i,j,esPrimo;
    
    j=0;
    int arreglo[N];
    
	//omp_set_num_threads(n);//seteamos cant de threads de OMP_NUM_THREADS
    #pragma omp parallel shared(j)
	{
        double tiempo=sampleTime();
        #pragma omp for schedule (guided) private(esPrimo)
        for ( i = 0; i < Rango; i++)
        {
            esPrimo=primo(i); // es privada de cada thread
            #pragma omp critical //zona critica actualizo arreglo y j que son compartidas
            if (esPrimo==1)
            {
                arreglo[j]=i;
                j++;  
            } 
        }
        tiempo = sampleTime() - tiempo;
        printf("Soy el thread número: %d  mi tiempo fue de: %f \n",omp_get_thread_num(),tiempo);
	}  

    printf("arregloPrimos = [ ");
 
    for ( i = 0; i < N; i++)
    {   
        printf("%d ", arreglo[i]);
    }
    printf("] \n");
    
    return 0;
    
}