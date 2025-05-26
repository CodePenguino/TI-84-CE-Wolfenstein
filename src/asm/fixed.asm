  assume adl=1
  section .text

public __fx2uint
__fx2uint:
	ld  iy,0
	add iy,sp

	ld  hl,(iy+3) ; Set hl to the function's argument
	ld  a,h
	ret
