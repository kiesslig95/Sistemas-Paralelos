#include <stdio.h>
int main()
{
  char matriz[3][4];
  for(int i=0; i<3; i++){
    for(int j=0; j<4; j++){
     printf("Elemento_%d_%d: %p ",i,j, &matriz[i][j]);
    }
  printf("/n");
}
return 0;
}
