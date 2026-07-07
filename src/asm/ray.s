    .assume adl=1

; a * bc, stored in hl (in fixed point)
.macro MUL8CODE
	; hl = a*c (fractional)
	ld h,a
	ld l,c
	mlt hl

	; hl /= 256
	ld l,h
	ld h,0

	; de = a*b (whole)
	ld d,a
	ld e,b
	mlt de

	add hl,de
.endm

    ; F_rayDirX is negative
SIDEDISTXMUL_NEG:
    MUL8CODE ; result stored in hl (F_sideDistX)

    ; e = stepX
    ld de,-32
    ret

    ; F_rayDirX is positive
SIDEDISTXMUL_POS:
    ; (-pos)-1
    neg
    dec a

    MUL8CODE

    ; e = stepX
    ld de,32
    ret

    ; F_rayDirY is negative
SIDEDISTYMUL_NEG:
    MUL8CODE ; result stored in hl (F_sideDistX)

    ; e = stepY
    ld de,-1
    ret

    ; F_rayDirY is positive
SIDEDISTYMUL_POS:
    ; (-pos)-1
    neg
    dec a

    MUL8CODE

    ; e = stepY
    ld de,1
    ret

; ------------------------------------------------

	.section .text._raycast
	.global _raycast
	.type _raycast, @function
_raycast:
    ld iy,0
    add iy,sp
	push ix

	; Setup for map pointer (ix)
	ld ix,_Map

    ; ---------- F_sideDistX setup ----------

    ; e = mapX (posX high byte)
	;ld de,0
	;ld de,(iy+18)
    ld e,(iy+16)
	ld d,32
	mlt de
	add ix,de    ; offset map pointer by mapX*32


    ; a = (uint8_t)posX
    ld a,(iy+15)

    ; bc = F_deltaDistX
    ld bc,(iy+9)

    ; d = F_rayDirX (highest byte)
    ld d,(iy+5)

    ; hl = F_sideDistX
    ; e = stepX
    bit 7,d
    call z,SIDEDISTXMUL_POS ; F_rayDirX is positive
    call nz,SIDEDISTXMUL_NEG ; F_rayDirX is negative

    ; a = mapX (posX high byte)
    ld a,(iy+16)

    ; ---------- F_sideDistY setup ----------
	; swap af & others
	ex af, af'
	exx

    ; de' = mapY (posY high byte)
	ld de,0
    ld e,(iy+19)
	add ix,de    ; offset map pointer by mapY


    ; a' = (uint8_t)posY
    ld a,(iy+18)

    ; bc' = F_deltaDistY
    ld bc,(iy+12)

    ; d' = F_rayDirY (highest byte)
    ld d,(iy+8)

    ; hl' = F_sideDistY
    ; e' = stepY
    bit 7,d
    call z,SIDEDISTYMUL_POS ; F_rayDirX is positive
    call nz,SIDEDISTYMUL_NEG ; F_rayDirX is negative

    ; a' = mapY (posY high byte)
    ld a,(iy+19)

	; unswap af & others
	ex af, af'
	exx

	; ---------- The loop ----------

	; F_sideDistY > F_sideDistX
	push af ; store a

	exx
	ld a,h
	exx
	scf ; carry = 0
	ccf
	cp a,h ; compare high bits (a-h) (F_sideDistY - F_sideDistX)
	jr c,SIDEDISTXBIGGER      ; jump if F_sideDistX is bigger
	jr nz,SIDEDISTYBIGGER     ; jump if high bits are not the same (skip check below)
	; compare low bits if high bits are equal
	exx
	ld a,l
	exx
	cp a,l ; compare low bits (a-l) (F_sideDistY - F_sideDistX)
	jr c,SIDEDISTXBIGGER ; jump if F_sideDistX is bigger

SIDEDISTXBIGGER:
	pop af    ; restore a

	add hl,bc ; F_sideDistX += F_deltaDistX
	;add ix,de ; map pointer += stepX

	ld hl,0
	ld l,(ix)

	pop ix
	ret

SIDEDISTYBIGGER:
	pop af    ; restore a

	ex af,af' ; Swap registers
	exx

	add hl,bc ; F_sideDistY += F_deltaDistY
	;add ix,de ; map pointer += stepY

	ex af,af' ; Unswap registers
	exx

	ld hl,0
	ld l,(ix)

	pop ix
    ret



	.section .rodata._Map
	.global _Map
	.balign 256
_Map:
	db 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1
	db 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	db 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	db 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	db 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	db 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	db 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	db 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	db 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	db 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
