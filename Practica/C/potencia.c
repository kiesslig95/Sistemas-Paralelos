#include <stdio.h>

void main(void){
char i;
int res=1;
int base=2;
for(i=1;i<30;i++){
  res*=base;
  printf("\n 2^%d= %d",i,res);
}

}
