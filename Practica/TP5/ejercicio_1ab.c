#include<stdio.h>
#include<stdlib.h>
#include <omp.h>

void main (int argc, char *argv[])
{

    int n = 10;
	omp_set_num_threads(n);//seteamos cant de threads de OMP_NUM_THREADS
	//cuando no declaro cantidad de hilos en la region paralela usa OMP_NUM_THREADS
	#pragma omp parallel
	{		
		printf("Hola mundo soy el thread número: %d de %d\n",omp_get_thread_num(), omp_get_num_threads());
	}

	/*
	ejercicio b

	if(argc==2) //Si hay un argumento, se utiliza como 'a'
	{
		n = atoi(argv[1]);
	}


	#pragma omp parallel num_threads(n)
	{		
		printf("Hola mundo soy el thread número: %d de %d\n",omp_get_thread_num(), n);
	}
	*/

	printf("----------------------------\n");
}