NAME = WOLF3D
ICON = icon.png
DESCRIPTION = "Wolfenstein raycaster by Rodrigo Agras"
COMPRESSED = YES
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

ifndef CEDEV
# $(error CEDEV environment path variable is not set)
# Swap this location to wherever you have CEDev installed
CEDEV ?= ../CEdev
#CEDEV ?= C:/CEdev
endif

include $(CEDEV)/meta/makefile.mk

all: run

run:
	CEmu --launch ./bin/WOLF3D.bin --send ./bin/WOLF3D.8xp ../CEmu/clibs.8xg --fullscreen 1
