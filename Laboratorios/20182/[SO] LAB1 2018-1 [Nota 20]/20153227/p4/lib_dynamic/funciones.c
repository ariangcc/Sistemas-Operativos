#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "struct.h"

void message(char **argv, char *m)
{
   fprintf(stderr,m,argv[0]);
   exit(1); 
}

void imprimeBloques(super_block sb, inode ino, int fd)
{  int n,k,i,offset,size,bsize,resto;
   char buffer[4*1024];

   size = ino.i_size;
   bsize = sb.s_block_size;
   n = size/bsize ;
   resto = size - bsize*n;

   if(n > 7) { 
	printf("Solo se imprimiran los 7 primeros bloques\n");
	n = 7;
	resto = 0;
   }	
   for(i=0;i < n;i++) {
	 offset = ino.i_zone[i]*sb.s_block_size;
	 lseek(fd,offset,SEEK_SET);
         read(fd,buffer,bsize);
         for(k=0;k<bsize;k++) printf("%c",buffer[k]);	 
   }
   if(resto){
      offset = ino.i_zone[n]*sb.s_block_size;
      lseek(fd,offset,SEEK_SET);
      read(fd,buffer,resto);
      for(k=0;k<resto;k++) printf("%c",buffer[k]);
   }   
}	

void leeInodo(super_block sb,int i,inode *ino, int fd)
{   int offset;
    offset = (i - 1)*sizeof(inode)+sb.s_block_size*(2 + sb.s_imap_blocks + sb.s_zmap_blocks); 
    lseek(fd,offset,SEEK_SET);
    read(fd,ino,sizeof(inode));
}	
