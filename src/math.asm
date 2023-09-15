  assume adl=1

  section .text
	public __getSinCos
__getSinCos:
	; NOTE: My eyes are bloodshot
	; returns a = sin/cos(a) * 128
	ld  c,a
	bit 7,a
	jr  z,$+4
	sub a,128
	bit 6,a
	jr  z,$+6
	ld  e,a
	ld  a,128
	sub a,e
	ld  de,0
	ld  e,a
	ld  hl,_SineTable
	add hl,de
	ld  a,(hl)
	bit 7,c
	ret z
	neg
	ret

  section .rodata
	public _SineTable
_SineTable:
	; sin(x) * 128
	db 0,3,6,9,13,16,19,22,25,28,31,34,37,40,43,46
	db 49,52,55,58,60,63,66,68,71,74,76,79,81,84,86,88
	db 91,93,95,97,99,101,103,105,106,108,110,111,113,114,116,117
	db 118,119,121,122,122,123,124,125,126,126,127,127,127,127,127,127,127