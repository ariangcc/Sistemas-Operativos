#include <stdio.h> // printf()
#include <sys/types.h> // para usar pid_t 
#include <sys/wait.h> // wait() , waitpid()
#include <unistd.h> //getpid
#include <stdlib.h> //contiene a system

int main(void){
    pid_t child; 
    int status; 

    if((child = fork())){
        fprintf(stderr,"Fork() devolvio %d por tanto soy el padre con ID =  %d\n", child, getpid());


    }
    else{
        fprintf(stderr,"Fork() devolvio %d por tanto soy el hijo con ID =  %d\n", child, getpid());
        //haciendo esto solo , solo toma en cuenta al proceso hijo pues el padre ya acabo
        //system("pstree > arbolprocesosmypid.txt");
 
    }
    /*
    TAREA 1: 
    
    a) modificar el programa para que el padre espere al hijo, de forma que
    cuando el hijo "tome la foto" con pstree, el padre salga en ella*/
        
        //haciendo esto ambos procesos imprimiran el arbol
        //system("pstree > arbolprocesos2mypid.txt");

    /*le indico al padre que debe esperar a que el proceso hijo  cambie de estado*/
    wait(&status);
    /*Le indico que solo si es el proceso hijo imprima el arbol esto porque quiero que el hijo sea el que
    'tome la foto' y como hice wait estoy seguro que mientras no acabe el proceso padre tampoco acabará y saldra 
    en la foto tambien, en este momento el padre se encuentra activo pero esperando a que el hijo acabe , y el 
    proceso hijo tam,bien se encuentra activo ejecutandose, por ello cuando saco la foto salen ambos*/
    if(child == 0){
        system("pstree > arbolprocesosmypid3.txt"); //(averiguar uso system)  
    }


    /*
    
    b) explique la presencia del proceso sh :: bueno sh es el lenguaje de programacion de comandos de shell y bash es una 
    implementación de sh, el punto es que estos son los que permiten ejecutar desde la linea de comandos 
    
    al parecer bash se usa cuando escribo en la terminal, toma toda la informacion y crea un proceso tipo bash 
    sh se inicia cuando usé system y una vez iniciado recien pude usar un comando, es decir primero abrio sh luego
    ya podia colocar comandos 
    
    c) como mostrar solo la rama correspondiente a un proceso usando su pid
        
        char comando[100];
        sprintf(comando,"pstree -s %d > arbolindividualmypid.txt", getppid());
        system(comando);

    d) Modifique el programa para que el archivo aprocesos.txt sea creado en el directorio donde se
        encuentra sus programas fuentes y no en el home

        a mi me sale todo en mi directorio, lo estoy haciendo desde visual estudio code(averiguar)
    */
    return 0;
}