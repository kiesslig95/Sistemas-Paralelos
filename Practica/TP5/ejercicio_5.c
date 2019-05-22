#include<stdlib.h>
#include<stdio.h>
#include <omp.h>
#define max(x,y)((x<y)?y:x)

void main ()
{
    int i,sum=0;
    int v[10000];

    for (i=0; i<10000;i++)
    {
        v[i]=i;
    }
	#pragma omp parallel for
		for(i=0;i<10000;i++)
        {
            if(sum<v[i])
            { 
                
                #pragma omp critical            
                sum=v[i];
          
                /*
                #pragma omp atomic           
                sum*=0;

                #pragma omp atomic           
                sum+=v[i];
                */
            }
        }
    printf("Resultado: %d \n",sum);

}