#include<stdio.h>
#include<stdlib.h>
#include <omp.h>

void main (int argc, char *argv[])
{

    int n = 10;

	if(argc==2) //Si hay un argumento, se utiliza como 'a'
	{
		n = atoi(argv[1]);
	}


	#pragma omp parallel num_threads(n)
	{		
		printf("Hola mundo soy el thread número: %d de %d\n",omp_get_thread_num(), n);
	}
	

	printf("------------CHAUUUUUUUUUU----------------\n");
}