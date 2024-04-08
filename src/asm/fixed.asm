  assume adl=0
  section .text

public __fx2uint
__fx2uint:
	; Load required variables
	ld  iy,0
	add iy,sp

	ld  hl,(iy+3) ; Set hl to the function's argument
	ld  a,h
	ret
