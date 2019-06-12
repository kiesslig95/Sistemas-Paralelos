#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])		
{
#define MSGLEN 2048
  int ITAG_A = 100,ITAG_B = 200; 
  int irank, i, idest, isrc, istag, iretag;
  float rmsg1[MSGLEN];
  float rmsg2[MSGLEN];
  MPI_Status recv_status;
  MPI_Request request; // para el Isend 

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &irank);  

  for (i = 0; i < MSGLEN; i++)
    {
      rmsg1[i] = 100;
      rmsg2[i] = -100;
    }
  if ( irank == 0 )
    { 
      idest  = 1;
      isrc   = 1;
      istag  = ITAG_A;
      iretag = ITAG_B;
    }
  else if ( irank == 1 )
    {
      idest  = 0;
      isrc   = 0;
      istag  = ITAG_B;
      iretag = ITAG_A;
    }
   /*
   Posibles soluciones:
   1) Poner el Send como no bloqueante Isend() y dejo el recv bloqueante ( syncronico == bloqueante en este caso)

   2) agregar if y else para diferenciar entre los rank donde el segundo rank=1 empieza recibiendo y luego envia 
 

    */   
  printf("Task %d has sent the message\n", irank);
  MPI_Isend(
    /* data         = */&rmsg1, 
    /* count        = */MSGLEN, 
    /* datatype     = */MPI_FLOAT, 
    /* destination  = */idest, 
    /* tag          = */istag, 
    /* communicator = */MPI_COMM_WORLD,
    /* request */&request); 

  MPI_Recv(
    /* data         = */&rmsg2, 
    /* count        = */MSGLEN, 
    /* datatype     = */MPI_FLOAT, 
    /* source       = */isrc, 
    /* tag          = */iretag, 
    /* communicator = */ MPI_COMM_WORLD,
    /* status       = */&recv_status);
  printf("Task %d has received the message\n", irank);
  MPI_Finalize();
}