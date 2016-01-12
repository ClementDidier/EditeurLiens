#!/bin/bash

#creation d'un fichier exemple exemple.o

if [ $# -eq 2 ]
then
	arm-eabi-as -o $1.o $1.s
	arm-eabi-as -o $2.o $2.s
	arm-eabi-ld -r -o exemple.o $1.o $2.o
else
	arm-eabi-as -o exemple.o $1.s
fi

#test sur nos fonctions

