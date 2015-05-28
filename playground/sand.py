#!/usr/bin/env python

import serial
import string
import time

print "Set up.."
port=serial.Serial("/dev/ttyAMA0",9600)
port.open()

print "Reading"
while True:
   #line = port.readline()
   #print line
   port.write("HI")
