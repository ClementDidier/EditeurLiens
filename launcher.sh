#!/bin/bash

source ~/.envarm/setenvarm.sh
source ~/.envarm/setenvarm.csh
./src/Programme_Editeur_Liens/configure
make all

./launcher $1

