  assume adl=1
  section .text

public _fxmul8
_fxmul8:
	ld iy,0
	add iy,sp

	ld a,(iy+3)
	ld de,(iy+6)

	; hl = a*e (fractional)
    ld h,a
    ld l,e
    mlt hl

	; hl /= 256
	ld l,h
	ld h,0

	; bc = a*d (whole)
    ld b,a
    ld c,d
    mlt bc

	add hl,bc

	ret

public __fx2uint
__fx2uint:
	ld  iy,0
	add iy,sp

	ld  hl,(iy+3) ; Set hl to the function's argument
	ld  a,h
	ret
