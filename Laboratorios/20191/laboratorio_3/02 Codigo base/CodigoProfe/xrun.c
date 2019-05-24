#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

void *xmalloc (size_t nbytes);
void visualize(const char* msg);
void *xrealloc(void * ptr, size_t size);
void xfree(void * ptr);

int main(void){
   //PARA VERIFICAR BEST FIT
   char *ptstring1;
   char *ptstring2;
   char *ptstring3;
   char *ptstring4;
   char *ptstring5;
   char *ptstring6;

   char *ptreall1;
   visualize("start");
   unsigned int char_size = sizeof(char);

   printf("*Nota: el elemento en [] representa el freep,\n la busqueda siempre comienza desde el nodo siguiente a freep\n\n");

   //generando  una lista con 3 nodos y el nodo base
   ptstring1 = (char*) xmalloc(80*char_size);
   visualize("solicitar 80 char");
   
   ptstring2 = (char*) xmalloc(100*char_size);
   visualize("solicitar 80 char");
    
   ptstring3 = (char*) xmalloc(120*char_size);
   visualize("solicitar 80 char"); 

   ptstring4 = (char*) xmalloc(140*char_size);
   visualize("solicitar 80 char"); 

   ptstring5 = (char*) xmalloc(160*char_size);
   visualize("solicitar 80 char"); 

   ptstring6 = (char*) xmalloc(190*char_size);
   visualize("solicitar 80 char"); 

   xfree(ptstring1);
   visualize("xfree(pstring1) de char[80]");

   xfree(ptstring3); 
   visualize("xfree(pstring3) de char[120]");

   xfree(ptstring5); 
   visualize("xfree(pstring5) de char[160]");
   

   // VERIFICACION PARA DISMINUCIÓN DE MEMORIA
   ptreall1 = (char*) xrealloc(ptstring2,33);
   visualize("realloc(pstring2)");

   // VERIFICACION PARA AUMENTO DE MEMORIA

   //ptstring5 = (char*) xmalloc(90*char_size);
   //visualize("solicitar 90 char");

   //ptstring3 = (char*) xmalloc(20*char_size);
   //visualize("solicitar 20 char");

   //Se puede notar claramente como cambia el punto de inicio de la busqueda al lugar con diferencia mas corta
   
   //mañana probar con Aligns
   exit(0);              
}



/*    
codigo original
{ unsigned int x,unidad,base;
  unsigned int *pt;

  unidad=1024; 
  x=0;

  do {
    base=pow(2,x)+.5;
    if((pt=(unsigned int *)xmalloc(base*unidad))) 
       fprintf(stdout,"Se solicitaron %d bytes y estan ubicados en %p\n",base*unidad,pt);
    else
       fprintf(stderr,"No hay suficiente memoria\n");       
    x++; }
  while(x<=6);      
*/


/*
   //PARA VERIFICAR NEXT FIT
   //PARA VERIFICAR FIRST FIT ES LO MISMO SOLO COMENTAR LINEA 151 Y 222
   char *ptstring1;
   char *ptstring2;
   char *ptstring3;
   char *ptstring4;
   char *ptstring5;
   char *ptstring6;

   visualize("start");
   unsigned int char_size = sizeof(char);

   printf("*Nota: el elemento en [] representa el freep,\n la busqueda siempre comienza desde el nodo siguiente a freep\n\n");

   //generando  una lista con 3 nodos y el nodo base
   ptstring1 = (char*) xmalloc(80*char_size);
   visualize("solicitar 80 char");
   
   ptstring2 = (char*) xmalloc(80*char_size);
   visualize("solicitar 80 char");
    
   ptstring3 = (char*) xmalloc(80*char_size);
   visualize("solicitar 80 char"); 

   ptstring4 = (char*) xmalloc(80*char_size);
   visualize("solicitar 80 char"); 

   xfree(ptstring1);
   visualize("xfree(pstring1) de char[80]");

   xfree(ptstring3); 
   visualize("xfree(pstring3) de char[80]");

   printf("\n\nLa lista ya esta generada ahora se realizan 2 busquedas \npara mostrar el funcionamiento de NextFit\n\n\n");
   
   ptstring5 = (char*) xmalloc(30*char_size);
   visualize("solicitar 30 char");

   ptstring6 = (char*) xmalloc(40*char_size);
   visualize("solicitar 40 char");

   //Se puede notar claramente como cambia el punto de inicio de la busqueda
*/

/*
   //PARA VERIFICAR BEST FIT y WORST FIT  
   char *ptstring1;
   char *ptstring2;
   char *ptstring3;
   char *ptstring4;
   char *ptstring5;
   char *ptstring6;

   visualize("start");
   unsigned int char_size = sizeof(char);

   printf("*Nota: el elemento en [] representa el freep,\n la busqueda siempre comienza desde el nodo siguiente a freep\n\n");

   //generando  una lista con 3 nodos y el nodo base
   ptstring1 = (char*) xmalloc(80*char_size);
   visualize("solicitar 80 char");
   
   ptstring2 = (char*) xmalloc(100*char_size);
   visualize("solicitar 80 char");
    
   ptstring3 = (char*) xmalloc(120*char_size);
   visualize("solicitar 80 char"); 

   ptstring4 = (char*) xmalloc(140*char_size);
   visualize("solicitar 80 char"); 

   ptstring5 = (char*) xmalloc(160*char_size);
   visualize("solicitar 80 char"); 

   ptstring6 = (char*) xmalloc(190*char_size);
   visualize("solicitar 80 char"); 

   xfree(ptstring1);
   visualize("xfree(pstring1) de char[80]");

   xfree(ptstring3); 
   visualize("xfree(pstring3) de char[120]");

   xfree(ptstring5); 
   visualize("xfree(pstring5) de char[160]");

   printf("\n\nLa lista ya esta generada ahora se realizan 2 busquedas \npara mostrar el funcionamiento de BestFit\n\n\n");
   
   ptstring5 = (char*) xmalloc(90*char_size);
   visualize("solicitar 90 char");

   ptstring3 = (char*) xmalloc(20*char_size);
   visualize("solicitar 20 char");

   //Se puede notar claramente como cambia el punto de inicio de la busqueda al lugar con diferencia mas corta

   exit(0);  

*/