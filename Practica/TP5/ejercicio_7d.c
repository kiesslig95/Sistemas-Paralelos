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
    #pragma omp parallel private(j)
    {
        int d =omp_get_thread_num();
        if(d>10) sleep(1);
        //#pragma omp for schedule (dynamic,2) private(j) //medio 3.03
        //#pragma omp for schedule (static) private(j) //peor 3.07
        #pragma omp for schedule (guided) private(j) //mejor 3.05 los primeros 10 se duermen no los carga de tareas{}
        for(i=0;i<10000000;i++)
            for (j= 0; j < 1000; j++); 
    }
    t = sampleTime() - t;
    printf("tiempo transcurrido: %f segundos\n", t);
    return 0;
}