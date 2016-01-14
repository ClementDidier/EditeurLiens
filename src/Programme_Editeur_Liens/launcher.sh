#!/bin/bash
source ~/.envarm/setenvarm.sh
source ~/.envarm/setenvarm.csh
./configure
make
chmod u+x ./arm_simulator
chmod u+x ./Simulation
./arm_simulator --gdb-port 6666 --trace-registers --trace-memory --trace-state &

echo -e "\n\n[TEST Simulation - example1.o] Lancement de la simulation\n\n"
./Simulation Examples_loader/example1.o

