#!/bin/bash

source ~/.envarm/setenvarm.sh
source ~/.envarm/setenvarm.csh
make all

./launcher $1

