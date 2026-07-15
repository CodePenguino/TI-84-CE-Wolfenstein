    .assume adl=1

	.include "src/asm/fixed.asm"

MapSize := 24

; a * bc, stored in hl (in fixed point)
.macro MUL8CODE_RAY
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
    MUL8CODE_RAY ; result stored in hl (F_sideDistX)

    ; de = stepX
    ld de,-MapSize
    ret

    ; F_rayDirX is positive
SIDEDISTXMUL_POS:
    ; (-pos)-1
    neg
    dec a

    MUL8CODE_RAY

    ; de = stepX
    ld de,MapSize
	cp a,a     ; ensure that z flag is set
    ret

    ; F_rayDirY is negative
SIDEDISTYMUL_NEG:
    MUL8CODE_RAY ; result stored in hl (F_sideDistX)

    ; de = stepY
    ld de,-1
    ret

    ; F_rayDirY is positive
SIDEDISTYMUL_POS:
    ; (-pos)-1
    neg
    dec a

    MUL8CODE_RAY

    ; de = stepY
    ld de,1
	cp a,a     ; ensure that z flag is set
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

    ; de = mapX (posX high byte)
	ld d,(iy+16)
	ld e,MapSize
	mlt de
	add ix,de    ; offset map pointer by mapX*MapSize


    ; a = (uint8_t)posX
    ld a,(iy+15)

    ; bc = F_deltaDistX
    ld bc,(iy+9)

    ; d = F_rayDirX (highest byte)
    ld d,(iy+5)

    ; hl = F_sideDistX
    ; de = stepX
    bit 7,d
    call z,SIDEDISTXMUL_POS ; F_rayDirX is positive
    call nz,SIDEDISTXMUL_NEG ; F_rayDirX is negative

    ; ---------- F_sideDistY setup ----------
	; swap registers
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
    call z,SIDEDISTYMUL_POS ; F_rayDirY is positive
    call nz,SIDEDISTYMUL_NEG ; F_rayDirY is negative

	; unswap registers
	exx

	; ---------- The loop ----------

	; F_sideDistY > F_sideDistX
RAYLOOP:
	exx
	ld a,h
	exx
	cp a,h ; compare high bits (a-h) (F_sideDistY - F_sideDistX)
	jr c,SIDEDISTXBIGGER      ; jump if F_sideDistX high bit is bigger (result above is negative)
	jr nz,SIDEDISTYBIGGER     ; jump if high bits are not the same (skip check below)
	; compare low bits if high bits are equal
	exx
	ld a,l
	exx
	cp a,l ; compare low bits (a-l) (F_sideDistY - F_sideDistX)
	jr c,SIDEDISTXBIGGER ; jump if F_sideDistX is bigger (result above is negative)
	; otherwise...

SIDEDISTYBIGGER:
	add hl,bc ; F_sideDistX += F_deltaDistX
	add ix,de ; map pointer += stepX

	; Check if we hit a wall
	ld a,(ix)
	or a,a
	jr z,RAYLOOP ; not hit a wall? jump back to beginning
	;jr ENDOFLOOP_Y ; else, jump to end

ENDOFLOOP_Y:
	; hl -= bc
	sbc hl,bc

	pop ix
    ret

SIDEDISTXBIGGER:
	exx

	add hl,bc ; F_sideDistY += F_deltaDistY
	add ix,de ; map pointer += stepY

	; Unswap registers
	exx

	; Check if we hit a wall
	ld a,(ix)
	or a,a
	jr z,RAYLOOP
	;jr ENDOFLOOP_X

ENDOFLOOP_X:
	; hl -= bc
	exx       ; swap
	sbc hl,bc

	pop ix
	ret

	.section .rodata._Map
	.global _Map
_Map:
	db 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1
	db 1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1
	db 1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
	db 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
