#!/usr/bin/python

# http://253.ccarh.org/handout/midiprotocol/
# http://www.midi.org/techspecs/midimessages.php

import time
import os
import sys

def midi(msg):
    port = open("/dev/ttyAMA0", "w")
    port.write(msg)
    port.close()

import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BOARD)
GPIO.setup(18, GPIO.OUT)
GPIO.output(18, True)
time.sleep(0.1)
GPIO.output(18, False)
time.sleep(0.1)
GPIO.output(18, True)

x = 1
x = 0.001
while True:
    #print 'A'
    midi("\x90\x20\x7f")
    time.sleep(x)
    #print 'B'
    midi("\x80\x20\x7f")
    time.sleep(x)
