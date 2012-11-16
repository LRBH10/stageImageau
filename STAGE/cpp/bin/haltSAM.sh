#!/bin/bash

echo "Stopping SAM: reseting relays ..."
./integratedDIO -t sam -a reset

echo "Stopping SAM: switching off ignition ..."
./integratedDIO -t sam -p 0 -a set
./integratedDIO -t sam -p 1 -a set

echo "Stopping SAM: sleeping 12 sec. ..."
sleep 12

echo "Stopping SAM: reseting relays ..."
./integratedDIO -t sam -a reset

echo "Stopping SAM: OK!"
