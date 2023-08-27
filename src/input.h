#pragma once

#include <keypadc.h>

static uint16_t __key_curr;

static inline bool is_key_pressed(uint16_t key)
{
	return __key_curr & key;
}

static inline void key_update()
{
	kb_Scan();
	__key_curr = kb_Data[7];
}