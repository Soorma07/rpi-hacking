#!/bin/bash

ROOT=`pwd`/..
LINUXDIR=${ROOT}/linux
TOOLDIR=${ROOT}/tools/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin

make -C ${LINUXDIR} ARCH=arm \
  CROSS_COMPILE=${TOOLDIR}/arm-bcm2708-linux-gnueabi- \
  SUBDIRS=`pwd` modules

scp *.ko pi@rpi:/home/pi
