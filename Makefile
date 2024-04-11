NAME = WOLF3D
ICON = icon.png
DESCRIPTION = "Woflenstein raycaster for the Ti84 Plus CE by Rodrigo Agras"
COMPRESSED = NO
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

ifndef CEDEV
# $(error CEDEV environment path variable is not set)
# Swap this location to wherever you have CEDev installed
#CEDEV ?= ../CEdev
CEDEV ?= C:/CEdev
endif

include $(CEDEV)/meta/makefile.mk
