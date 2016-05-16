import time

callbackFunc = None
filename = "coordinates.txt"

def setCallback(func):
   global callbackFunc
   callbackFunc = func

def getNumberOfSteps():
   fil = open(filename, "r")
   lines = 0
   for line in fil:
      if not line.lstrip().startswith("#"):
         lines += 1
   return lines

def coordParsing(thick):
   fp = open(filename, "r")
   
# reads from file and returns x, y, z

   # Changing the z coord. 
   # if value is 3, we know its "go home, thickness of 0.75"
   # if value is 4, we know its "go home, thickness of 1.5"
   if (thick == 1) :
      thick = 3
   else:
      thick = 4
   if callbackFunc != None:
      print("Starting callback")
      callbackFunc(0, 0, 0, "Returning home")
      print("Done callback")

   time.sleep(0.5)
   currentLetter = None
   # while wait
   # poll once from serial port for info arrival
   # return home when [cancel is clicked]/[finsihed] toHOG(0,0,0)
   for coord in fp:
      if (coord.lstrip().startswith("#")):
         # This line is a comment. No real HOG data
         currentLetter = coord.lstrip().split()[1].strip()
      else:
         intCoord = map(int, coord.split(","))
      
         # send data to HOG
         if callbackFunc != None:
            callbackFunc(intCoord[0], intCoord[1], intCoord[2], "Letter: " + currentLetter)

         time.sleep(0.3)

         
   # Go home, you're drunk.
   if callbackFunc != None:
      callbackFunc(0, 0, 0, "Returning home")

   time.sleep(0.5)

   return

