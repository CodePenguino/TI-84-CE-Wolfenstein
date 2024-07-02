#pragma once

#include <keypadc.h>

static kb_key_t __key_prev;
static kb_key_t __key_curr;

static inline bool key_pressed(const kb_key_t key) {
	return __key_curr & key;
}

static inline bool key_tapped(const kb_key_t key) {
	return !(__key_prev & key) && (__key_curr & key);
}

static inline bool key_held(const kb_key_t key) {
	return (__key_prev & key) && (__key_curr & key);
}

static inline bool key_let_go(const kb_key_t key) {
	return (__key_prev & key) && !(__key_curr & key);
}

static inline void key_update() {
	__key_prev = __key_curr;
	__key_curr = kb_AnyKey();
}
