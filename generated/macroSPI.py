import os
import math

# To generate the SPI settings, instead of using a macro
# like a normal person, I'm generating all of the arrays
# inside of this python script. This is because I couldn't
# get the macros working in the assembly file, so this is
# the next best thing I could think of

list_arguments = []

SPI_BIT = 0x80
SPI_VALUE = 0x00

def SPI_START():
	SPI_BIT = 0x80
	SPI_VALUE = 0x00

def SPI_END():
	if SPI_BIT != 0x80:
		list_arguments.append("db " + str(SPI_VALUE) + "	; SPI_END")

def SPI_PARAM(param):
	global SPI_BIT
	global SPI_VALUE
	list_arguments.append("db " + 
		str(((param * SPI_BIT) >> 8) | (SPI_VALUE | SPI_BIT)) + "	; SPI_PARAM(" + str(hex(param)) + ")")
	SPI_VALUE = (param * SPI_BIT) & 0xFF

	if SPI_BIT == 0x01:
		SPI_END()
		SPI_START()
	else:
		SPI_BIT = SPI_BIT >> 1

def SPI_PARAM16(param):
	SPI_PARAM(param >> 8)
	SPI_PARAM(param & 0xFF)

def SPI_CMD(cmd):
	global SPI_BIT
	global SPI_VALUE
	list_arguments.append("db " + str(((cmd * SPI_BIT) >> 8) | SPI_VALUE) + "	; SPI_CMD(" + str(hex(cmd)) + ")")
	SPI_VALUE = (cmd * SPI_BIT) & 0xFF

	if SPI_BIT == 0x01:
		SPI_END()
		SPI_START()
	else:
		SPI_BIT = SPI_BIT >> 1

# Code that is converted to assembly...
SPI_START()
SPI_CMD(0xB0)
SPI_PARAM(0x02)
SPI_PARAM(0xF0)
SPI_CMD(0x2C)
SPI_CMD(0xB0)
SPI_PARAM(0x12)
SPI_PARAM(0xF0)
SPI_END()

# Writes all of the code onto the file output ---------
if os.path.exists("spi.txt"):
	os.remove("spi.txt")

with open("spi.txt", "a") as f:
	for x in range (0, len(list_arguments)):
		f.write(list_arguments[x] + "\n")
	
