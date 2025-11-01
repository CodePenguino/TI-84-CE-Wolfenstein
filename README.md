# What?
This is a project made in an attempt to create a pseudo-3D raycaster for the TI-84 Plus CE, hopefully running as fast as possible.

# How do I compile this?
1. Install the TI-84 Plus CE Toolchain on your computer, following the instructions on its official website: https://ce-programming.github.io/toolchain/static/getting-started.html
2. After cloning the repository, go to src/gfx and run the convimg yaml file. This should convert any images in the project to textures usable by the project.
3. Go into the makefile and set the CEDEV variable to wherever you extracted the CEDev folder in step 1.
4. Run the makefile

In theory you should now have an 8xp file in a bin directory which should be able to run on a TI-84 Plus CE emulator or calculator.

# Credits
Some of the rendering code is modified from the following project by Patrick Davidson. My version should run faster due to unrolled loops being used to render the walls rather than the traditional loops used in his project, as well as hardware register settings to halve the horizontal resolution.
https://www.cemetech.net/downloads/files/1588/x1588
