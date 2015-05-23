import serial
import os

port = serial.Serial("/dev/ttyUSB0", baudrate=9600, timeout=2)
#port = serial.Serial("/dev/tty.usbserial-A6026OUJ", baudrate=9600)

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

   # Go home, you're drunk.
   print ("program begins")
   toHOG(toBinary(0), toBinary(0), toBinary(0))
   print("first")
   found = port.read(1)
   print found
   while found.find('#') < 0:
      found = port.read(1)
      print found
   print "start coord"
   # while wait
   # poll once from serial port for info arrival
   # return home when [cancel is clicked]/[finsihed] toHOG(0,0,0)
   for coord in fp:
      coord = map(toBinary, map(int, coord.split(",")))
      print coord
   
      # send data to HOG
      toHOG(coord[0], coord[1], coord[2])
      found = port.read(1)
      print "1 " + found
      while found.find('@') < 0:
         found = port.read(1)
         print "2 " +found
         # wait until something in buffer
         
      print "3 " +found

   # Go home, you're drunk.
   toHOG(toBinary(0), toBinary(0), toBinary(0))
   print("done")
   while port.read(1).find('#') < 0:
      pass
   print "done done"

   return

def test():
   print "SUCCESS"


main()
#test()
