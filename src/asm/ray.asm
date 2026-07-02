    assume adl=1

    section .text

public _raycast
_raycast:
    ld iy,0
    add iy,sp

    ; a = (uint8_t)posX
    ld a,(iy+18)

    ; de = F_deltaDistX
    ld de,(iy+12)

    ; ---------------------------------------------
    ;    hl = fxmul((uint8_t)posX, F_deltaDistX)
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
    ; ---------------------------------------------


    ret
