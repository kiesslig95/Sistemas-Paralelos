#include <stdio.h>
int main(){
  int *p,a,n;
  a=33;
  p=&a;
  n=1;
  p= p + n*sizeof(int);;
  printf ("%d \n",a);
  printf("%ls \n",p);
  return 0;
}
