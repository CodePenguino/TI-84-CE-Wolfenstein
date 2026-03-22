#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "gfx.h"

static uint8_t* texture_full_data;
static uintptr_t texture_offset;
static uint8_t* texture_data;

static inline void texture_init() {
	// Aligns texture data so last 8 bits are all 0 (fixes alignment bug)
	texture_full_data = malloc(4096 + 256);
	texture_offset = 256 - ((uintptr_t)texture_full_data % 256);

	memcpy((void*)(texture_full_data + texture_offset), door_texture_data, sizeof(door_texture_data));
	texture_data = (uint8_t*)(texture_full_data + texture_offset);
}

static inline void texture_disable() {
	free(texture_full_data);
}
