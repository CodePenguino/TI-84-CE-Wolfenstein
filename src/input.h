#pragma once

#include <ti/getcsc.h>

static sk_key_t __key_curr;

static inline bool key_pressed(const sk_key_t key)
{
	return __key_curr == key;
}

static inline void key_update()
{
	__key_curr = os_GetCSC();
	// kb_Scan();
	// __key_curr = kb_Data[7];
}