#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#define BUFFSIZE 10000
#define PATHSIZE 100
#define N 4

int main(int argc, char const *argv[]){
    pid_t child,pid_padre;
    int fd[4][2];
    int i;
    
    FILE *pf;
    int file;
    int newFile;
    int npipe = 0;
    
    char buffer[BUFFSIZE];
    char nombre[PATHSIZE];
    char path[PATHSIZE];
    char cadena[PATHSIZE];

    pid_padre=getpid();

    for(i = 0; i < N; i++)
        pipe(fd[i]);
    

    for(i=0;i<N; ++i)
        if((child=fork())== 0) break;
    
    
    // el padre escribe los nombres de los ar4chivos en los pipes
    if(getpid()==pid_padre){
        printf("%d",argc);
        pf=popen("ls",argv[1]); 
        if( pf == NULL ) {
            perror("Error al abrir archivo en padre");
            return(-1);
        }     
        while(fgets(cadena,sizeof(cadena),pf)!=NULL) {	
            write(fd[npipe][1],cadena,sizeof(cadena));
            npipe++;
            if(npipe == 3) npipe = 0;
        }        
    }
     
    if(i==0){
        //se lee del pipe respectivo, se abre el archivo
        read(fd[i][0],nombre,sizeof(nombre));
        sprintf(path,"%s/%s",argv[1],nombre);        
        file=open(path,O_RDONLY);
        //creo un archivo con el mismo nombre, escribo
        newFile=open(nombre,O_WRONLY | O_APPEND);
        while(read(file,buffer,BUFFSIZE) != 0)
            write(newFile,buffer,BUFFSIZE);                        
        close(file);
        close(newFile);
    }
    else if(i==1){
        //se lee del pipe respectivo, se abre el archivo
        read(fd[i][0],nombre,sizeof(nombre));
        sprintf(path,"%s/%s",argv[1],nombre);        
        file=open(path,O_RDONLY);
        //creo un archivo con el mismo nombre, escribo
        newFile=open(nombre,O_WRONLY | O_APPEND);
        while(read(file,buffer,BUFFSIZE) != 0)
            write(newFile,buffer,BUFFSIZE);                        
        close(file);
        close(newFile);
    }
    else if(i==2){
         //se lee del pipe respectivo, se abre el archivo
        read(fd[i][0],nombre,sizeof(nombre));
        sprintf(path,"%s/%s",argv[1],nombre);        
        file=open(path,O_RDONLY);
        //creo un archivo con el mismo nombre, escribo
        newFile=open(nombre,O_WRONLY | O_APPEND);
        while(read(file,buffer,BUFFSIZE) != 0)
            write(newFile,buffer,BUFFSIZE);                        
        close(file);
        close(newFile);
    }
    else if(i==3){
        //se lee del pipe respectivo, se abre el archivo
        read(fd[i][0],nombre,sizeof(nombre));
        sprintf(path,"%s/%s",argv[1],nombre);        
        file=open(path,O_RDONLY);
        //creo un archivo con el mismo nombre, escribo
        newFile=open(nombre,O_WRONLY | O_APPEND);
        while(read(file,buffer,BUFFSIZE) != 0)
            write(newFile,buffer,BUFFSIZE);                        
        close(file);
        close(newFile);
    }

    return 0;
}