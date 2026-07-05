NAME = WOLF3D
ICON = icon.png
DESCRIPTION = "Experimental wolfenstein raycaster"
COMPRESSED = YES
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)

#ifndef CEDEV
# $(error CEDEV environment path variable is not set)
# Swap this location to wherever you have CEDev installed
#CEDEV ?= ../CEdev
#CEDEV ?= C:/CEdev
#endif

#include $(CEDEV)/meta/makefile.mk

#all : run

run:
	CEmu --launch ./bin/WOLF3D.bin --send ./bin/WOLF3D.8xp ../clibs.8xg --no-reset
