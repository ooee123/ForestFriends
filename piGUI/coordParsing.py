import serial
import os

# for the pi
#port = serial.Serial("/dev/ttyUSB0", baudrate=9600, timeout=2)
# for the mac
#port = serial.Serial("/dev/tty.usbserial-A6026OUJ", baudrate=9600)
# for testing
port = 1
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
   

def coordParsing(thick):
   print(thick)
   fp = open(filename, "r")
   
# reads from file and returns x, y, z

   # Go home, you're drunk.
   print ("data transfer begins")
   # Changing the z coord. 
   # if value is 3, we know its "go home, thickness of 0.75"
   # if value is 4, we know its "go home, thickness of 1.5"
   if (thick == 1) :
      thick = 3
   else :
      thick = 4
   toHOG(toBinary(0), toBinary(0), toBinary(thick))
   print("first")
   while port.read(1).find('#') < 0:
      pass

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
      while found.find('@') < 0:
         found = port.read(1)
         # wait until something in buffer
         
   # Go home, you're drunk.
   toHOG(toBinary(0), toBinary(0), toBinary(0))
   print("done")
   while port.read(1).find('#') < 0:
      pass
   print "done done"

   return

def test(thick):
   if (thick == 1) :
      thick = 3
   else :
      thick = 4
   print "SUCCESS " + str(thick)

#coordParsing(10)
#main()
#test(10)
