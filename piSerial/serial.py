import serial
def stripLine(line):
   tokens = line.split(',')
   splitTokens = []
   for t in tokens:
      splitTokens.append(t.strip(' ()'))
   print(splitTokens)
   return splitTokens 

letterWindowSize = 100
letterBurstSize = 10
coordinateFileName = "coordinates.txt"

# End user configuration #
coordinates = open(coordinateFileName)
port = serial.Serial("/dev/ttyS0", baudrate=9600, timeout=3.0)
windowFilled = 0

while True:
   if port.inwaiting() > 0:
      port.read(port.inwait())
c = 0
while c < 100:
   coord = coordinates.readline()
   tokens = splitLine(coord)
   bytesToSend = bytearray(6)
   bytesToSend[0] = tokens[0] / 256
   bytesToSend[1] = tokens[0] % 256
   bytesToSend[3] = tokens[1] / 256
   bytesToSend[4] = tokens[1] % 256
   bytesToSend[5] = tokens[2] / 256
   bytesToSend[6] = tokens[2] % 256
   serial.write(bytesToSend)
