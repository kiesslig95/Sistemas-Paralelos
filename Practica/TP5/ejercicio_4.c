#include<stdlib.h>
#include<stdio.h>
#include <omp.h>

void main ()
{
    int i,sum=0;
    int v[10000];

    for (i=0; i<10000;i++)
    {
        v[i]=i;
    }
	#pragma omp parallel for reduction(max:sum)
		for(i=0;i<10000;i++)
        {
            if(sum<v[i]){
                sum=v[i];
            }
        }
    printf("Resultado: %d \n",sum);

}