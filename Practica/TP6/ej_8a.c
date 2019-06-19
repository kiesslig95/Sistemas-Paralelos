#include <stdio.h>
#include <mpi.h>
#include <time.h>

#define N 10
#define size_vector 4

double sampleTime() {
 struct timespec tv;
 clock_gettime(CLOCK_MONOTONIC_RAW, &tv);
 return ((double)tv.tv_sec+((double)tv.tv_nsec)/1000000000.0);
}

int main(int argc, char *argv[])		
{
    
  int ITAG_A = 100,ITAG_B = 200; 
  int irank,i,j, idest, isrc, istag, iretag,X;
  float A[size_vector];
  float B[size_vector];
  int carga=0;
  double tiempo = sampleTime();
  MPI_Status recv_status;
  MPI_Request request; // para el Isend 

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &irank);  

  if (irank==0)
  {
      //printf("cargando A 0");
      for (i = 0; i < size_vector; i++)
      {
          A[i]=1;
      }
      idest  = 1;
      isrc   = 1;
      istag  = ITAG_A;
      iretag = ITAG_B;
      //carga=1;
      
  }else if (irank==1)
  {
      //printf("cargando A 1")
      for ( i = 0; i < size_vector; i++)
      {
          A[i]=2;
      }
      idest  = 0;
      isrc   = 0;
      istag  = ITAG_B;
      iretag = ITAG_A;
      //carga=1;
  }
 
  for (i = 0; i < N; i++)
  {
    if (irank==0)
    {
        printf("Task %d has sent the message\n", irank);
        MPI_Send(
            /* data         = */&A, 
            /* count        = */size_vector, 
            /* datatype     = */MPI_FLOAT, 
            /* destination  = */idest, 
            /* tag          = */istag, 
            /* communicator = */MPI_COMM_WORLD);

        MPI_Recv(
                /* data         = */&B, 
                /* count        = */size_vector, 
                /* datatype     = */MPI_FLOAT, 
                /* source       = */isrc, 
                /* tag          = */iretag, 
                /* communicator = */ MPI_COMM_WORLD,
                /* status       = */&recv_status);
        printf("Task %d has received the message\n", irank);
    }else
    {
        if (irank==1)
        {
            MPI_Recv(
                /* data         = */&B, 
                /* count        = */size_vector, 
                /* datatype     = */MPI_FLOAT, 
                /* source       = */isrc, 
                /* tag          = */iretag, 
                /* communicator = */ MPI_COMM_WORLD,
                /* status       = */&recv_status);  

            printf("Task %d has received the message\n", irank); 

            printf("Task %d has sent the message\n", irank);
            MPI_Send(
                /* data         = */&A, 
                /* count        = */size_vector, 
                /* datatype     = */MPI_FLOAT, 
                /* destination  = */idest, 
                /* tag          = */istag, 
                /* communicator = */MPI_COMM_WORLD);

           
        }
        
    }
    printf("vecto A=( ");
    for ( j= 0; j < size_vector; j++)
    {
        printf("%f ",A[j]);
    }
    printf(" )\n");

    printf("vecto B=( ");
    for ( j= 0; j < size_vector; j++)
    {
        printf("%f ",B[j]);
    }
    printf(") \n");
    
    int aux=0;
    for ( j = 0; j < size_vector; j++)
    {
        aux=A[j];
        X += aux*aux*aux;
    }
    for ( j = 0; j < size_vector; j++)
    {
        A[j]=X+B[j];
    }

    
  }
  
  MPI_Finalize();
    tiempo = sampleTime() - tiempo;
    printf("tiempo transcurrido no Solapado: %f segundos\n..........................................\n", tiempo);
  return 0;
}