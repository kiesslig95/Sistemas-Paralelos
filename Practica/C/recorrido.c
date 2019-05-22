#include <stdio.h>
int main(){
 int  m[3][4];
 int i,j,k ,a;
 int *p=(int*)&m; 
 for (i=0; i<12;i++){
  *(p+i)=i;
  printf("%d \n",*(p+i));
 }
 for(j=0;j<4;j++){
  for(k=0; k<3;k++){
    printf("%d \n", *(p+(j+k*4)));
  }
 }
return 0;
}
  
