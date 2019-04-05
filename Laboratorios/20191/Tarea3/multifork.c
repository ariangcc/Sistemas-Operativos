#include <sys/types.h> // para usar pid_t 
#include <sys/wait.h> // wait() , waitpid()
#include <stdio.h> // printf()
#include <stdlib.h> //contiene a system
#include <unistd.h> //getpid

#define DISPLAY1 "PID INDUK** ** pid (%5.5d) ** ***********\n"
#define DISPLAY2 "val1(%5.5d) -- val2(%5.5d) -- val3(%5.5d)\n"

// ESTE EJEMPLO SE REPITE EN EL LABORATORIO PRELIMINAR

int main(void){
	pid_t val1, val2, val3;
	//se imprime el pid del proceso actual
    printf(DISPLAY1, (int) getpid());
    //hago que se envie directamente a la salida estandar 
    fflush(stdout);

    //con pid = -1 significa que va a esperar el cambio de estado de cualquier proceso hijo igual al wait()
	val1 = fork();
	waitpid(-1,NULL,0);

	val2 = fork();
	waitpid(-1,NULL,0);

	val3 = fork();
	waitpid(-1,NULL,0);
    /*
    
        Tarea 3: 
        a) Haciendo uso de los datos proporcionados por el programa multifork.c elabore un esquema que
            represente el árbol de procesos generados por el proceso padre. 

            el arbol esta en mi cuaderno rojo analisis 3
        
        b) En el programa multifork.c cuál es la interpretación de su salida.
            Analizando el codigo: 
            analizando con detenimiento se concluye que al no haber condicionales por ningun lado todos los procesos
            llegaran a imprimir el ultimo mensaje de alguna u otra forma. la ejecucion se realiza de la siguiente forma 
            1. primero se imprimime el pid del proceso padre
            2. con cada proceso hijo creado se hace que el padre espere a que este espere en esa posicion para seguir 
                ejecutandose
            3. finalmente esto se traduce en ocho procesos que se ejecutaran hasta llegar a la impresion de los valores 
                cada padre espera a que su hijo acabe y continua con la siguiente linea, si tiene que crear un nuevo hijo 
                lo hace y nuevamente se coloca a la espera de que este nuevo hijo acabe esto brinda un orden a la impresion

            para este tipo de ejercicios es mejor hacer grafico y analizar detenidamente 
    */
	printf(DISPLAY2, (int)val1, (int)val2, (int)val3);
}