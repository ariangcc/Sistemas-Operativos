preproceso:
		cpp hola.c > hola.i

compilar: hola.i
	gcc -Wall -S hola.i -o hola.s

enlazar: hola.s
	as hola.s -o hola.o

ejecutable: hola.o
	gcc hola.o
