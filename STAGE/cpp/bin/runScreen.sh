#!/bin/bash

screen -dmS station_screen
sleep 1
screen -S station_screen -X screen ~/bin/runStation.sh
screen -p 0 -S station_screen -X kill
