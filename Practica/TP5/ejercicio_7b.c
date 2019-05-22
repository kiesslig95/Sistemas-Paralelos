#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

double sampleTime() {
 struct timespec tv;
 clock_gettime(CLOCK_MONOTONIC_RAW, &tv);
 return ((double)tv.tv_sec+((double)tv.tv_nsec)/1000000000.0);
}

int main(){
    int i,j;
    double t;
    t = sampleTime();
    int n = 4;
	omp_set_num_threads(n);//seteamos cant de threads de OMP_NUM_THREADS
    #pragma omp parallel for schedule (dynamic,10) private(j) //mejor 3.03
    //#pragma omp parallel for schedule (static,11) private(j) //peor 3.07
    //#pragma omp parallel for schedule (guided,10) private(j) //medio 3.05
        for(i=0;i<10000000;i++){
            if (i%2==0)
                for (j= 0; j < 100; j++);
            else
                for (j= 0; j < 1000; j++); 
        } 
        t = sampleTime() - t;
        printf("tiempo transcurrido: %f segundos\n", t);
    return 0;
    
}