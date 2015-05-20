import serial
import os

port = serial.Serial("/dev/ttyUSB0", baudrate=9600, timeout=2)

filename = "coordinates.txt"

# reads from file and returns x, y, z
def readCoord(fp):
   return  map(int,fp.readline().split(",")) 

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


def main():
   fp = open(filename, "r")
   
   coord = readCoord(fp)
   print coord
   coord = map(toBinary, coord)
   print coord
   
   toHOG(coord[0], coord[1], coord[2])

   # main passes readCoord a file pointer

   # while wait
   # poll once from serial port for info arrival
   # return home when [cancel is clicked]/[finsihed] toHOG(0,0,0)
   return




main()
