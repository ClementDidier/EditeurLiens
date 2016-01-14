#!/bin/bash

source ~/.envarm/setenvarm.sh
./src/Programme_Editeur_Liens/configure
make all
chmod u+x ./test_affichage_sections
chmod u+x ./test_API_read
chmod u+x ./test_API_write

# Lancement de l'affichage des headers de sections
echo -e "\n\n----------------------------------------------------------------------------------\n"
echo -e "\n\n[TEST - example1.o] Affichage des détails des entêtes de sections\n"
echo -e "----------------------------------------------------------------------------------\n"
./test_affichage_sections examples/example1.o

echo -e "\n\n----------------------------------------------------------------------------------\n"
echo -e "\n\n[TEST API - example1.o] Affichage du header, des headers de sections et des symboles\n"
echo -e "----------------------------------------------------------------------------------\n"
./test_API_read examples/example1.o


echo -e "\n\n----------------------------------------------------------------------------------\n"
echo -e "[TEST API - example1.o] Affichage du header, des headers de sections et des symboles\n"
echo -e "----------------------------------------------------------------------------------\n"
./test_API_write examples/example1.o
