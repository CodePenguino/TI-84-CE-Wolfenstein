	.assume adl=1

; hl = de * a
.macro MUL8CODE
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
.endm

	.section .text._fxmul8abs
	.global _fxmul8abs
	.type _fxmul8abs, @function
_fxmul8abs:
	ld iy,0
	add iy,sp

	ld a,(iy+3)
	ld de,(iy+6)

NORMALMUL8:
	MUL8CODE

	ret

	.section .text._fxmul8
	.global _fxmul8
	.type _fxmul8, @function
_fxmul8:
	ld iy,0
	add iy,sp

	ld a,(iy+3)
	ld de,(iy+6)

	ld c,0

	; Check if int8 is positive
CHECK1MUL8:
	bit 7,a
	jr z,CHECK2MUL8
	neg
	inc c

CHECK2MUL8:
	bit 7,d
	jr z,AFTER_CHECK2MUL8

	; invert de
	scf
	ccf
	sbc hl,hl
	sbc hl,de
	ex de,hl

	inc c

AFTER_CHECK2MUL8:
	; Don't invert result
	bit 0,c
	jr z,NORMALMUL8

	MUL8CODE

	;scf ; reset carry flag
	;ccf
	or a,a
	ex de,hl
	sbc hl,hl
	sbc hl,de ; hl is inverted

	ret

	.section .text._fxmul24abs
	.global _fxmul24abs
	.type _fxmul24abs, @function
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

.macro MUL24CODE
	push de
	; ------ Whole bit of a ------
	; hl = b*d
	ld h,b
	ld l,d
	mlt hl
	ld h,l
	ld l,0

	; de = b*e
	ld d,b
	mlt de

	add hl,de ; add to result

	; ------ Fractional bit of a ------
	;ld de,(iy+6)
	;ld d,a
	pop de

	push de
	; de = d*c (whole)
	ld e,c
	mlt de

	add hl,de ; add to result

	; load e
	pop de
	;ld e,(iy+6) ; 4 cycles

	; de = c*e (fractional)
	ld d,c
	mlt de
	; de /= 256
	ld e,d
	ld d,0
	add hl,de ; add to result
.endm

; hl = bc * de
	.section .text._fxmul24
	.global _fxmul24
	.type _fxmul24, @function
_fxmul24:
	ld iy,0
	add iy,sp

	ld bc,(iy+3) ; bc = first
	ld de,(iy+6) ; de = second

FXMUL24_FROMASM: ; used to call only from assembly...
	ld a,0

CHECK1:
	bit 7,b
	jr z,CHECK2

	or a,a
	; invert bc if it's negative
	sbc hl,hl
	sbc hl,bc

	ld bc,0
	ld b,h
	ld c,l
	ld (iy+3),c ; 4 cycles
	;scf
	;ccf ; ensure carry = 0
	or a,a

	inc a

CHECK2:
	bit 7,d
	jr z,AFTER_CHECK2

	; invert de if it's negative
	or a,a
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

	MUL24CODE
	ret

INVMUL24:
	MUL24CODE

	; Negate the result
	;ccf
	or a,a
	ex de,hl
	sbc hl,hl
	sbc hl,de

	ret

	.section .text.__fx2uint
	.global __fx2uint
	.type __fx2uint, @function
__fx2uint:
	ld  iy,0
	add iy,sp

	ld  hl,(iy+3) ; Set hl to the function's argument

	ret
