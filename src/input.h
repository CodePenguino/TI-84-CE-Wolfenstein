#pragma once

#include <keypadc.h>

static kb_key_t __key_curr;

static inline bool key_pressed(const kb_key_t key)
{
	return __key_curr & key;
}

static inline void key_update()
{
	kb_Scan();
	__key_curr = kb_AnyKey();
}