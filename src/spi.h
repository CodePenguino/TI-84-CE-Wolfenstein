#pragma once

// Rendering things on the screen is pretty time-consuming, so in order to
// speed things up, this halves the resolution using some SPI magical-address
// fuckery

// The SPI is a piece of hardware that controls how things are displayed on
// the screen. There currently aren't any C functions to control it on the
// TI-84 Plus CE Toolchain (as of May 2024), so I have to roll my own with this
// one...

// Most of the code is modified from calc86maniac's source code for tiboyce
// https://github.com/calc84maniac/tiboyce/blob/cc5a1fac6fc4840e1483fc6a38c48aff2143549a/setup.asm#L4245
// Basically the only difference is that in his version, he uses archive memory
// to store everything, while my version of the code just uses modified example code
// to get the job done

#ifdef __cplusplus
extern "C" {
#endif
extern void set_scaled_mode();
#ifdef __cplusplus
}
#endif