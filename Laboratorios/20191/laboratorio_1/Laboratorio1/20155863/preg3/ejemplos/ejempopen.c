#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {    
    FILE *pf;
    int i=1;
    char cadena[30],otracadena[35];
    
    pf=popen("ls","r");    
    while(fgets(cadena,sizeof(cadena),pf)!=NULL) {	
       sprintf(otracadena,"%d) %s",i,cadena);
       i++;
       printf("%s",otracadena);
    }
       
    exit(0);
}      
