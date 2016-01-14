#!/bin/bash
source ~/.envarm/setenvarm.sh
source ~/.envarm/setenvarm.csh
./configure
make
chmod u+x ./arm_simulator
chmod u+x ./Simulation

echo -e "\n\n[TEST Simulation $1] Lancement de la simulation\n\n"
for file in Examples_loader/*.o
do
	./arm_simulator --gdb-port 6666 --trace-registers --trace-memory --trace-state & > /dev/null
	./Simulation $file
done
