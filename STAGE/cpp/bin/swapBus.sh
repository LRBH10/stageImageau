#!/bin/bash

if [ x$1 == x ]
then
    bus=ttySBus2
else
    bus=$1
fi

cd /dev
mv ttySBus ttySBusTmp
mv $bus ttySBus
mv ttySBusTmp $bus

ls -alF ttySBus*
cd -


