# What?
This is a project made in an attempt to create a pseudo-3D raycaster for the TI-84 Plus CE, hopefully running as fast as possible.

# How do I compile this?
1. Install the TI-84 Plus CE Toolchain on your computer, following the instructions on its official website: https://ce-programming.github.io/toolchain/static/getting-started.html
2. After cloning the repository, go to src/gfx/texture and run the convimg yaml file. Also run the align.py file with python. This should convert any images in the project to usable textures and align them in memory properly.
3. Run the makefile

In theory you should now have an 8xp file in a bin directory which should be able to run on a TI-84 Plus CE emulator or calculator.

# Credits
Some of the rendering code is modified from the following project by Patrick Davidson. My version should run faster due to unrolled loops being used to render the walls rather than the traditional loops used in his project, as well as hardware register settings (SPI) to halve the horizontal resolution, as described below.

https://www.cemetech.net/downloads/files/1588/x1588

SPI rendering code was based on the following wiki page for setting hardware settings...

https://wikiti.brandonw.net/index.php?title=84PCE:Ports:D000

...and the following for setting the resolution properly (specifically the "spiSetupDoubleScale" section):

https://github.com/calc84maniac/tiboyce/blob/master/setup.asm
