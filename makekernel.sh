#!/bin/bash

HERE=`pwd`
LINUXDIR=${HERE}/linux
TOOLDIR=${HERE}/tools/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin

gunzip < ${HERE}/config.gz > ${LINUXDIR}/.config

# make the RPi kernel and modules
(
cd ${LINUXDIR}
#make -j 6 ARCH=arm CROSS_COMPILE=${TOOLDIR}/arm-bcm2708-linux-gnueabi- clean
make -j 6 ARCH=arm CROSS_COMPILE=${TOOLDIR}/arm-bcm2708-linux-gnueabi- oldconfig
make -j 6 ARCH=arm CROSS_COMPILE=${TOOLDIR}/arm-bcm2708-linux-gnueabi-
make -j 6 ARCH=arm CROSS_COMPILE=${TOOLDIR}/arm-bcm2708-linux-gnueabi- modules
)
