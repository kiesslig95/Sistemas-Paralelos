#include <stdio.h>
#include<stdlib.h>
#include <omp.h>
#define N 4

double f(double x){
    return x;
}

int main(){
    double h,b,a,aprox,x;
    int i;
    a=1;
    b=4;
    h=(b-a)/N;
    aprox=(f(a)+f(b))/2.0;
	omp_set_num_threads(N);//seteamos cant de threads de OMP_NUM_THREADS
    //#pragma omp parallel for private(x) 
    #pragma omp parallel for private(x) reduction(+:aprox)
    for (i= 1;i<N; i++) 
    {
        x= a+i*h;
        //otra opcion sin el reduction
        //#pragma omp atomic 
        aprox+=f(x);
    }
    aprox= h * aprox;
    printf("Resultado: %f \n",aprox);
    return 0;
}
    
    
