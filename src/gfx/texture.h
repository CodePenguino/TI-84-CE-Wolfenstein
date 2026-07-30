#pragma once

#include <stdint.h>
#include "texture/door_texture.h"

// Pointer to the section of the texture used for drawing
uint8_t* texture_pointer = (uint8_t*)door_texture_data;
uint8_t* texture_shadow_pointer = (uint8_t*)(door_texture_data+4096);
