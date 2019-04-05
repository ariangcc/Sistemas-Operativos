/*
 * =====================================================================================
 *
 *       Filename:  catFile.c
 *
 *    Description:  Lee y muestra el contenido de un archivo que se encuentra en el 
 *                  directorio raiz y cuyo nombre se pasa como parametro
 *        Version:  1.0
 *        Created:  16/11/11 10:42:04
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alejandro T. Bello Ruiz (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "struct.h"

void message(char **, char *);
void leeInodo(super_block, int, inode *, int);
void imprimeBloques(super_block, inode, int);

int fd;

int main(int narg, char *argv[]) 
{   int n,i;
    super_block sb;
    inode ino;
    entry_dir endir;    

    if(narg != 3) message(argv,"Usage:%s <file_system> <file_name>\n");
    if((fd = open(argv[1],O_RDONLY))< 0) perror("Can not read file sytem");
    lseek(fd,1024,SEEK_SET);
    read(fd,&sb,sizeof(super_block));
    leeInodo(sb,1,&ino, fd);
    n = ino.i_size / sizeof(entry_dir);
    lseek(fd,sb.s_firstdatazone*sb.s_block_size,SEEK_SET);
    i=0;
    while(i < n) {
	 read(fd,&endir,sizeof(entry_dir));
         if(endir.d_inodo) {
		 if(!strcmp(endir.d_name,argv[2])) {
	            leeInodo(sb,endir.d_inodo,&ino, fd);   		 
		    break;
	         }		 
		 i++;
	 } 
    }
    if(i==n) message(argv,"Archivo %s no se encuentra");
    imprimeBloques(sb,ino, fd);     
    close(fd);
    exit(0);	    
} 
