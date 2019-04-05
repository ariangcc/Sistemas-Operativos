all: main.o employee.o address.o
	gcc -o myProgram main.o employee.o address.o

main.o : main.c address.h employee.h
	gcc -c main.c

employee.o: employee.c address.h
	gcc -c employee.c

address.o: address.c
	gcc -c address.c

clean:
	rm -rf *.o
	rm myProgram
