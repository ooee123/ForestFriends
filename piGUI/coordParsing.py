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

def getNumberOfSteps():
   fil = open(filename, "r")
   lines = 0
   for line in fil:
      if not line.lstrip().startswith("#"):
         lines += 1
   return lines

# converts a number to a 2 byte binary integer
def toBinary(integer):
   array = bytearray(4)
   array[0] = (integer >> 24) & 0xFF
   array[1] = (integer >> 16) & 0XFF
   array[2] = (integer >> 8) & 0xFF
   array[3] = integer & 0xFF 

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

def coordParsing(thick):
   print(thick)
   fp = open(filename, "r")
   
# reads from file and returns x, y, z

   # Changing the z coord. 
   # if value is 3, we know its "go home, thickness of 0.75"
   # if value is 4, we know its "go home, thickness of 1.5"
   if (thick == 1) :
      thick = 3
   else:
      thick = 4
   print thick
   #toHOG(toBinary(0), toBinary(0), toBinary(int(thick)))
   toHOG(toBinary(0), toBinary(0), toBinary(0))
   if callbackFunc != None:
      callbackFunc(0, 0, 0, "Returning home")
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
      if (coord.lstrip().startswith("#")):
         # This line is a comment. No real HOG data
         currentLetter = coord.lstrip().split()[1].strip()
      else:
         intCoord = map(int, coord.split(","))
         binCoord = map(toBinary, intCoord)
      
         # send data to HOG
         toHOG(binCoord[0], binCoord[1], binCoord[2])
         if callbackFunc != None:
            callbackFunc(intCoord[0], intCoord[1], intCoord[2], "Letter: " + currentLetter)

         found = port.read(1)
         print found
         while found.find('@') < 0:
            found = port.read(1)
            print found
            # wait until something in buffer
         
   toHOG(toBinary(0), toBinary(0), toBinary(0))
   if callbackFunc != None:
      callbackFunc(0, 0, 0, "Returning home")
   print("done")
   while port.read(1).find('#') < 0:
      pass
   print "done done"

   return

#coordParsing(2)
#main()
#test(10)
