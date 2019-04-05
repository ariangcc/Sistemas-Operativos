/*
 * =====================================================================================
 *
 *       Filename:  dirRaiz.c
 *
 *    Description:  Lee y muestra el contenido del directorio raíz
 *
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
#include "struct.h"

void message(char **, char *);

int main(int narg, char *argv[]) 
{   int fd,offset,n,i;
    super_block sb;
    inode ino;
    entry_dir endir;    

    if(narg != 2) message(argv,"Usage:%s <file_system>\n");
    if((fd = open(argv[1],O_RDONLY))< 0) perror("Can not read file sytem");
    lseek(fd,1024,SEEK_SET);
    read(fd,&sb,sizeof(super_block));
    offset = sb.s_block_size*(2 + sb.s_imap_blocks + sb.s_zmap_blocks);  
    lseek(fd,offset,SEEK_SET);
    read(fd,&ino,sizeof(inode));
    n = ino.i_size / sizeof(entry_dir);
    lseek(fd,sb.s_firstdatazone*sb.s_block_size,SEEK_SET);
    i=0;
    while(i < n) {
	 read(fd,&endir,sizeof(entry_dir));
         if(endir.d_inodo) {
		 printf("%s\n",endir.d_name);
		 i++;
	 } 
    }	    
    close(fd);
    exit(0);	    
}      	  
