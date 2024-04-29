NAME = WOLF3D
ICON = icon.png
DESCRIPTION = "Woflenstein raycaster for the Ti84 Plus CE by Rodrigo Agras"
COMPRESSED = NO
ARCHIVED = NO

CFLAGS = -Wall -Wextra -O3
CXXFLAGS = -Wall -Wextra -O3

# ----------------------------

ifndef CEDEV
# $(error CEDEV environment path variable is not set)
# Swap this location to wherever you have CEDev installed
#CEDEV ?= ../CEdev
CEDEV ?= C:/CEdev
endif

include $(CEDEV)/meta/makefile.mk

all: run

run:
	C:/Users/Rodrigo\ Agras/Downloads/windows_CEmu/CEmu.exe --launch ./bin/WOLF3D.bin --send ./bin/WOLF3D.8xp --fullscreen 1
#	CEmu --launch ./bin/WOLF3D.bin --send ./bin/WOLF3D.8xp --fullscreen 1
