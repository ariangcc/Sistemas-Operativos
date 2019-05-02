#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/wait.h>


double final; 
int pid_padre;
int cons;
void crea_arbol(int);


int main(int narg, char* argv[]){
    int n = atoi(argv[1]); 
    final = pow(2,(n-1)); //te dice que a partir de esos valores ya se encuentra en el ultimo nivel
    //al ser final el indicador del ultimo nivel, es una proporcion de qué tanto debo hacer dormir a los hijos
    cons = final; // funciona con cualquier valor mayor o igual a final
    pid_padre = getpid();
    crea_arbol(1);
    return 0;
}

void crea_arbol(int x){
    
    char cadena[60];
    sprintf(cadena,"Soy el proceso %2d con pid %d y ppid %d\n",x,getpid(), getppid());
    write(1,cadena,strlen(cadena)); 

    //quiero que el ultimo de los ultimos imprima y siempre este valor es final*2-1
    if(x == final*2-1){
        char comando[100];
        //si se hace pstree -p -s pid se imprime desde el pid para delante y para atrás 
        //sprintf(comando,"pstree -p -s %d > binarytree.txt" ,getpid());
        //si se hace pstree -p pid se imprime del para adelante nomas
        sprintf(comando,"pstree -p %d > binarytree.txt" ,pid_padre);
        system(comando);
    }
    //tiene que ser antes de return para asegurar que todos salgan en la foto
    if(x >= final) {
        return;
    }

    /*una vez creado el hijo debemos  enviarlo a dormir antes que se acabe y muera
        pero hay algo que se debe considerar y es el tiempo de dormir, es logico que 
        aquellos que se crean primero moriran antes es a ellos a quienes se debe 
        enviar a dormir mas tiempo, mientras x crece -> el tiempo decrece 
        esto se logra haciendo dormir a cada hijo de cada nivel un tiempo const/x
    */
    if(!fork()){
        crea_arbol(2*x);
        sleep(cons/x);
        exit(EXIT_SUCCESS);    
    }


    if(!fork()){
        crea_arbol(2*x+1);
        sleep(cons/x);
        exit(EXIT_SUCCESS);    
    }

    wait(NULL);
    wait(NULL);
}
    /*
        Puntos importantes del codigo: 

        - se debe hacer np = np*2 y np = np*2 +1 debido a que si comenzamos con 1 el hijo imprime 2*1 
            y el siguiente hijo imprime 2*1+1  de igual manera cuando el hijo tenga que crear su propio
            hijo este hijo imprima 2*2 y el otro hijo 2*2 +1  
        
        - algo que me impide avanzar es que al crear dos procesos por vez el padre espera a que alguno 
         de ellos acabe, sin embargo esto no afecta pues la impresion se realiza en el momento exacto de la 
         creacion del proceso y si bien no se sabe que hijo acabará antes , se sabe que primero se crea uno 
         y luego el otro 
           

        - analizando el codigo se nota que no hay forma de contraolar esto , (preguntar)    
    
        tarea4 

        a)Modifique el programa para obtener el archivo binarytree.txt de forma que contenga la salida del
            programa pstree, mostrando el árbol de procesos.

            haciendo el analisis se puede decir que si quiero que todos los procesos existentes salgan en 
            la foto, el ultimo proceso creado debe tomarla sin embargo para el momento en que el ultimo proceso
            esta siendo creado se han perdido varias de las ramas creadas, preguntar(no se ha llegado a la 
            solucion )

            haciendo el analisis, nuevamente gracias a bello se nota que, si algun hijo hace la foto con -s esto va 
            producir que solo su rama se imprima mas no sus 'hermanos' , esto no es lo se busca, lo que se quiere
            es que a partir del proceso padre salgan todos sus hijos en adelante, con pstree pid del padre, pero 
            esta foto tiene que ser tomada despues de la creacion del ultimo hijo y ademas se debe asegurar que 
            todos los hijos se encuentren procesando esto se hace con sleep() que hace dormir un programa poor una
            cierta cantidad de tiempo especificada, en segundos.


    
        b)Escriba un programa que cree un árbol de procesos, semejante al anterior, pero que el recorrido
            que se realiza para la creación de procesos sea siguiendo el esquema primero-en-profundidad.

            

    */



