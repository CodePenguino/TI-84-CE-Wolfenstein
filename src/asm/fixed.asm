  assume adl=1
  section .text

public _fxmul8abs
_fxmul8abs:
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

public _fxmul24abs
_fxmul24abs:
	ld iy,0
	add iy,sp

	ld a,(iy+4)
	ld de,(iy+6)

	; ------ Whole bit of a ------
	; hl = a*d
	ld h,a
	ld l,d
	mlt hl
	ld h,l
	ld l,0

	; bc = a*e
	ld b,a
	ld c,e
	mlt bc

	add hl,bc ; add to result

	; ------ Fractional bit of a ------
	ld a,(iy+3)

	; bc = a*e (fractional)
	ld b,a
	ld c,e
	mlt bc
	; bc /= 256
	ld c,b
	ld b,0
	add hl,bc ; add to result

	; bc = a*d (whole)
	ld b,a
	ld c,d
	mlt bc

	add hl,bc ; add to result

	ret

public _fxmul24
_fxmul24:
	ld iy,0
	add iy,sp

	ld bc,(iy+3) ; bc = first
	ld de,(iy+6) ; de = second

	ld a,0

CHECK1:
	bit 7,b
	jr z,CHECK2

	; invert bc if it's negative
	sbc hl,hl
	sbc hl,bc

	ld b,h
	ld c,l
	;scf
	ccf ; ensure carry = 0

	inc a

CHECK2:

	bit 7,d
	jr z,AFTER_CHECK2

	; invert de if it's negative
	sbc hl,hl
	sbc hl,de
	ex de,hl
	ld (iy+6),e ; 4 cycles

	inc a

AFTER_CHECK2:
	; check if inversion at end is needed
	bit 0,a
	ld a,d
	jr nz,INVMUL24

NORMALMUL24:
	; ------ Whole bit of a ------
	; hl = a*d
	ld h,b
	ld l,d
	mlt hl
	ld h,l
	ld l,0

	; de = a*e
	ld d,b
	mlt de

	add hl,de ; add to result

	; ------ Fractional bit of a ------
	;ld de,(iy+6)
	ld d,a

	; de = d*c (whole)
	ld e,c
	mlt de

	add hl,de ; add to result

	; load e
	ld e,(iy+6) ; 4 cycles

	; de = c*e (fractional)
	ld d,c
	mlt de
	; de /= 256
	ld e,d
	ld d,0
	add hl,de ; add to result

	ret

INVMUL24:
	; ------ Whole bit of a ------
	; hl = a*d
	ld h,b
	ld l,d
	mlt hl
	ld h,l
	ld l,0

	; de = a*e
	ld d,b
	mlt de

	add hl,de ; add to result

	; ------ Fractional bit of a ------
	;ld de,(iy+6)
	ld d,a

	; de = d*c (whole)
	ld e,c
	mlt de

	add hl,de ; add to result

	; load e, negate it
	ld e,(iy+6)

	; de = c*e (fractional)
	ld d,c
	mlt de
	; de /= 256
	ld e,d
	ld d,0
	add hl,de ; add to result

	; Negate the result
	;ccf
	ex hl,de
	sbc hl,hl
	sbc hl,de

	ret

public __fx2uint
__fx2uint:
	ld  iy,0
	add iy,sp

	ld  hl,(iy+3) ; Set hl to the function's argument

	ret
