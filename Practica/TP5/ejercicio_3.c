#include<stdio.h>
#include<stdlib.h>
#include <omp.h>

void main (int argc, char *argv[])
{
    int x, y;
    x = 1;
    y = 1;

    #pragma omp parallel private(y) shared(x)
    {
        x = 2;
        y = 2;

        #pragma omp parallel private(x) shared(y)
        {
            int z = 1;
            x = 3;
            y = 3;
        }
        
        printf("x=%d y =%d\n", x, y);
    }
    
    printf("x=%d y=%d\n", x,y);
}