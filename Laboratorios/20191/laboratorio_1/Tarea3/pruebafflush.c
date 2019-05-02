#include <sys/types.h> // para usar pid_t 
#include <sys/wait.h> // wait() , waitpid()
#include <stdio.h> // printf()
#include <stdlib.h> //contiene a system
#include <unistd.h> //getpid

int main(void){

int numero;
   
   printf("Introduce un numero: ");
   fflush(stdout);
   /* sin el fflush
   ** En este momento la cadena "Introduce un numero: " se 
   ** encuentra en el bufer de la salida estandar y *NO* esta 
   ** garantizado que esta cadena se envie a la salida estandar 
   ** *ANTES* de ejecutarse la siguiente llamada a scanf.
   */
   
   /* Lectura del numero introducio por el usuario */
   scanf("%d", &numero);
   
   printf("El numero introducio fue %d\n", numero);
   
   return EXIT_SUCCESS;
    

}