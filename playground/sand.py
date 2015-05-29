#  ## !/usr/bin/env python

import serial
import string
import time

print "Set up.."
port = serial.Serial("/dev/ttyAMA0", baudrate = 9600)

print "Reading"
while True:
   port.write("\r\nSay something:")
   string =  port.readline()
   #print line
   port.write("\r\nYou sent:" + repr(string))
