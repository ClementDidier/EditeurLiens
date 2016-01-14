#!/bin/bash

source ~/.envarm/setenvarm.sh
./src/Programme_Editeur_Liens/configure
make all
chmod u+x ./test_affichage_sections
chmod u+x ./test_API_read
chmod u+x ./test_API_write
chmod u+x ./test_renumeration

echo -e "\n\n----------------------------------------------------------------------------------"
echo -e "[TEST - $1] Affichage des détails des entêtes de sections"
echo -e "----------------------------------------------------------------------------------\n"
./test_affichage_sections $1

echo -e "\n\n----------------------------------------------------------------------------------"
echo -e "[TEST API - $1] Affichage du header, des headers de sections et des symboles"
echo -e "----------------------------------------------------------------------------------\n"
./test_API_read $1


echo -e "\n\n----------------------------------------------------------------------------------"
echo -e "[TEST API - $1] Ecriture du header, des headers de sections et des symboles"
echo -e "----------------------------------------------------------------------------------\n"
./test_API_write $1

echo -e "\n\n----------------------------------------------------------------------------------"
echo -e "[TEST - $1] Affichage des détails des entêtes de sections"
echo -e "----------------------------------------------------------------------------------\n"
./test_renumeration $1
