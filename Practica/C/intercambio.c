#include <stdio.h>

void intercambio( int *a, int *b){
 int temp= *a;
 *a=*b;
 *b=temp;
}
int main(){
 int x=1, y=2;
 printf("%d %d \n", x,y);
 intercambio( &x, &y);
 printf("%d %d \n", x, y);
 return 0; 
}
