import serial
import os

# for the pi
#port = serial.Serial("/dev/ttyUSB0", baudrate=9600, timeout=2)
# for the mac
#port = serial.Serial("/dev/tty.usbserial-A6026OUJ", baudrate=9600)
# for testing
port = 1
filename = "coordinates.txt"
callbackFunc = None

# converts a number to a 2 byte binary integer
def toBinary(integer):
   array = bytearray(2)
   array[0] = integer/256
   array[1] = integer%256
   return array

def setCallback(func):
   global callbackFunc
   callbackFunc = func

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

   # Changing the z coord. 
   # if value is 3, we know its "go home, thickness of 0.75"
   # if value is 4, we know its "go home, thickness of 1.5"
   if (thick == 1) :
      thick = 3
   else :
      thick = 4
   print thick
   #toHOG(toBinary(0), toBinary(0), toBinary(int(thick)))
   toHOG(toBinary(0), toBinary(0), toBinary(0))
   if callbackFunc != None:
      callbackFunc(x, y, z, "Returning home")
   print("first")
   # Wait until # symbol comes
   while port.read(1).find('#') < 0:
      pass

   currentLetter = None
   print "start coord"
   # while wait
   # poll once from serial port for info arrival
   # return home when [cancel is clicked]/[finsihed] toHOG(0,0,0)
   for coord in fp:
      if (coord.trim().startswith("#")):
         # This line is a comment. No real HOG data
         currentLetter = coord.trim()[1:].trim()
         print("Working on letter %s" % currentLetter)
      else:
         coord = map(toBinary, map(int, coord.split(",")))
      
         # send data to HOG
         toHOG(coord[0], coord[1], coord[2])
         if callbackFunc != None:
            callbackFunc(x, y, z, currentLetter)

         found = port.read(1)
         print found
         while found.find('@') < 0:
            found = port.read(1)
            print found
            # wait until something in buffer
         
   # Go home, you're drunk.
   toHOG(toBinary(0), toBinary(0), toBinary(0))
   if callbackFunc != None:
      callbackFunc(x, y, z, "Returning home")
   print("done")
   while port.read(1).find('#') < 0:
      pass
   print "done done"

   return

#coordParsing(2)
#main()
#test(10)
