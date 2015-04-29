all:
	avr-g++ -L/usr/arduino/lib -I/usr/arduino/include -I/usr/share/arduino/hardware/arduino/variants/standard -I/usr/share/arduino/hardware/arduino/cores/arduino -I. -mmcu=atmega328p -DF_CPU=16000000L -Os -o main.elf CPFF.c /usr/share/arduino/hardware/arduino/cores/arduino/*.c /usr/share/arduino/hardware/arduino/cores/arduino/*.cpp
	avr-objcopy -O ihex -R .eeprom main.elf main.hex
	avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:main.hex
