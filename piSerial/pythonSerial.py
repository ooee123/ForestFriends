import serial
import os
import threading

# for testing
port = 1
# for the pi
port = serial.Serial("/dev/ttyS0", baudrate=9600)

# converts a number to a 4 byte binary integer
def toBinary(integer):
   array = bytearray(4)
   array[0] = (integer >> 24) & 0xFF
   array[1] = (integer >> 16) & 0XFF
   array[2] = (integer >> 8) & 0xFF
   array[3] = integer & 0xFF 

   return array

# send x y z values to HOG
def toHOG(x, y, z):
   port.write(x) 
   port.write(y) 
   port.write(z) 
   return

def readPort():
   while True:
      reading = port.readline().decode()
      print(reading)

def main():
   thread = threading.Thread(target=readPort)
   thread.start()
   print("Ready to input commands. Type in 3 numbers")
   print("1st Number, number up to 32 bits. X-axis")
   print("2nd Number, number up to 32 bits. Y-axis")
   print("3rd Number, 0 for START, 1 for LINE, 2 for MOVE, 3 for 0.75in board cut, 4 for 1.5in board cut")
   print("All numbers are parsed for you and transmitted as a 32 bit number to HOG")
   while True:
      user = raw_input()
      ints = map(int, user.split())

      print("Got {0}, {1}, {2}".format(*ints))
      toHOG(toBinary(ints[0]), toBinary(ints[1]), toBinary(ints[2]))

main()
