#include <stdio.h>
#include <stdlib.h>
#include "fraccion.h"

int main(){
	fraccion x, y; 
	x.num = 5; x.den = 7; y.num = 11; y.den = 13; //Inicializando fracciones
	fraccion fsuma = suma(x,y);
	fraccion fresta = resta(x,y);
	fraccion fprod = producto(x,y);
	fraccion fdiv = division(x,y);

	printf("Fsuma:\n\tNumerador: %d\n\tDenominador: %d\n", fsuma.num, fsuma.den);
	printf("Fresta:\n\tNumerador: %d\n\tDenominador: %d\n", fresta.num, fresta.den);
	printf("Fprod:\n\tNumerador: %d\n\tDenominador: %d\n", fprod.num, fprod.den);
	printf("Fdiv:\n\tNumerador: %d\n\tDenominador: %d\n", fdiv.num, fdiv.den);

	return 0;
}
