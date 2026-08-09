	.assume adl=1

	.section .text.__getSinCos
	; Definitely not copy-pasted directly from the graphx source code ;)
	.global __getSinCos
	.type __getSinCos, @function
__getSinCos:
	; returns sin(a) * 128
	ld  c,a    ; c = a (original angle)
	bit 7,a
	jr  z,$+4  ; jump if a is 0-127
	sub a,128  ; a -= 128
	bit 6,a
	jr  z,$+6  ; jump if a is 128-191
	ld  e,a    ; e = angle
	ld  a,128  ; a = 128
	sub a,e    ; a -= angle (a = 128 - angle)
	ld  hl,_SineTable
	ld  l,a
	ld  a,(hl)
	bit 7,c    ; return if input is 0-127
	ret z
	neg        ; negate if greater
	ret

	.section .text.__getSinCosNeg
	.global __getSinCosNeg
	.type __getSinCosNeg, @function
__getSinCosNeg:
	; returns sin(a) * 128
	ld  c,a    ; c = a (original angle)
	bit 7,a
	jr  z,$+4  ; jump if a is 0-127
	sub a,128  ; a -= 128
	bit 6,a
	jr  z,$+6  ; jump if a is 128-191
	ld  e,a    ; e = angle
	ld  a,128  ; a = 128
	sub a,e    ; a -= angle (a = 128 - angle)
	ld  hl,_SineTable
	ld  l,a
	ld  a,(hl)
	bit 7,c    ; return if input is 0-127
	ret nz
	neg        ; negate if greater
	ret

	.section .text._abs24
	.global _abs24
	.type _abs24, @function
_abs24:
	; return absolute value of an int24
	ld iy,0
	add iy,sp ; carry = 0

    ld hl,(iy+3)

	bit 7,h   ; positive? return
	ret z

	ex de,hl
	sbc hl,hl
	sbc hl,de

	ret

	; Table is byte aligned for easier look-up math
	.section .rodata._SineTable
	.global _SineTable
	.balign 256
_SineTable:
	; sin(x) * 128
	db 0,3,6,9,13,16,19,22,25,28,31,34,37,40,43,46
	db 49,52,55,58,60,63,66,68,71,74,76,79,81,84,86,88
	db 91,93,95,97,99,101,103,105,106,108,110,111,113,114,116,117
	db 118,119,121,122,122,123,124,125,126,126,127,127,127,127,127,127,127
