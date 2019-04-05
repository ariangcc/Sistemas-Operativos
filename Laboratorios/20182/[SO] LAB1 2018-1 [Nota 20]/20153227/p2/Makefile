main: main.c fraccion.h suma.o resta.o producto.o division.o
	gcc main.c suma.o resta.o producto.o division.o -o main

suma.o: suma.c fraccion.h
	gcc -c suma.c -o suma.o

resta.o: resta.c fraccion.h
	gcc -c resta.c -o resta.o

producto.o: producto.c fraccion.h
	gcc -c producto.c -o producto.o

division.o: division.c fraccion.h
	gcc -c division.c -o division.o
