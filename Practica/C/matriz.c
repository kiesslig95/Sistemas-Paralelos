#include <stdio.h>
int main()
{
   int matriz[3][4]= {{6,2,5,7},{3,0,0,1},{2,8,5,4}};
   int  copia[3][4];
   for(int i=0; i<3;i++)
   {
      for(int j=0; j<4;j++)
      {
         copia[i][j]=matriz[i][j];
         printf("%d  ",copia[i][j]);
       }
       printf("\n");
   }
}
