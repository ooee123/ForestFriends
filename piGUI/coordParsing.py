import serial
import os

#port = serial.Serial("/dev/ttyUSB0", baudrate=9600, timeout=2)
port = serial.Serial("/dev/tty.usbserial-A6026OUJ", baudrate=9600)

filename = "coordinates.txt"

# converts a number to a 2 byte binary integer
def toBinary(integer):
   array = bytearray(2)
   array[0] = integer/256
   array[1] = integer%256;
   return array

# send x y z values to HOG
def toHOG(x, y, z):
   port.write(x) 
   port.write(y) 
   port.write(z) 
   return

#change cancel flag
def cancelFlag():
   home = true
   

def main():
   fp = open(filename, "r")
   
# reads from file and returns x, y, z

   # while wait
   # poll once from serial port for info arrival
   # return home when [cancel is clicked]/[finsihed] toHOG(0,0,0)
   for coord in fp:
      coord = map(toBinary, map(int, coord.split(",")))
      print coord
   
      # send data to HOG
      toHOG(coord[0], coord[1], coord[2])
      foundZ = port.read(1)
      print "1 " + foundZ
      while foundZ.find('Z') < 0:
         foundZ = port.read(1)
         print "2 " +foundZ
         # wait until something in buffer
         
      print "3 " +foundZ

   # Go home, you're drunk.
   toHOG(toBinary(0), toBinary(0), toBinary(0))
   print("done")
   #while True:
   #   print port.read(1)

   return




main()