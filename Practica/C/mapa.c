#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
int hola(int x){
 if (x < 100000){
 x++;
 if ((x == 1) || (x == 100000)){
 printf("Ejecución %d. Enter para continuar\n", x);
 getchar();
 }
 hola(x);
 }
}
int main(){
 printf("PID: %d\n", getpid());
 hola(0);
 return 0;
}
