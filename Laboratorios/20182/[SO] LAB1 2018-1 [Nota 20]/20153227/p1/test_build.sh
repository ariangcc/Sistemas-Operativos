#!/bin/bash

echo -e "\n1. touch everything - everything should build"
touch *.c *.h
make
sleep 2

echo -e "\n2. touch nothing - nothing should build"
make
sleep 2

echo -e "\n3. touch address.h - main and employee should build"
touch address.h
make
sleep 2

echo -e "\n4. touch main.cpp - only main.o should build"
touch main.c
make
sleep 2

echo -e "\n5. touch employee.cpp - only employee.o should build"
touch employee.c
make
sleep 2

echo -e "\n6. touch address.cpp - only address.o should build"
touch address.c
make

echo -e "Removing myProgram and all object (.o) files from current directory"
rm myProgram *.o 
