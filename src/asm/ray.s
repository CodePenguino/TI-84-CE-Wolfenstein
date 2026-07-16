    .assume adl=1

	.include "src/asm/fixed.asm"

MapSize := 24

    ; F_rayDirX is negative
SIDEDISTXMUL_NEG:
    MUL8CODE ; result stored in hl (F_sideDistX)

    ; bc = stepX
    ld bc,-MapSize
    ret

    ; F_rayDirX is positive
SIDEDISTXMUL_POS:
    ; (-pos)-1
    neg
    dec a

    MUL8CODE

    ; bc = stepX
    ld bc,MapSize
	cp a,a     ; ensure that z flag is set
    ret

    ; F_rayDirY is negative
SIDEDISTYMUL_NEG:
    MUL8CODE ; result stored in hl (F_sideDistX)

    ; bc' = stepY
    ld bc,-1
    ret

    ; F_rayDirY is positive
SIDEDISTYMUL_POS:
    ; (-pos)-1
    neg
    dec a

    MUL8CODE

    ; bc' = stepY
    ld bc,1
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

    ; de = F_deltaDistX
    ld de,(iy+9)

    ; bc = F_rayDirX
    ld bc,(iy+3)

    ; hl = F_sideDistX
    ; bc = stepX
    bit 7,b
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

    ; de' = F_deltaDistY
    ld de,(iy+12)

    ; bc' = F_rayDirY
    ld bc,(iy+6)

    ; hl' = F_sideDistY
    ; bc' = stepY
    bit 7,b
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
	add hl,de ; F_sideDistX += F_deltaDistX
	add ix,bc ; map pointer += stepX

	; Check if we hit a wall
	ld a,(ix)
	or a,a
	jr z,RAYLOOP ; not hit a wall? jump back to beginning
	;jr ENDOFLOOP_Y ; else, jump to end

ENDOFLOOP_Y:
	; hl -= de
	sbc hl,de

	; bc = hl (F_perpWallDist)
	ld bc,0
	ld a,c ; a = 0 (for multiplication later...)
	ld b,h
	ld c,l

	push hl

	; de = F_rayDirY
	ld de,(iy+6)

	; hl = bc*de
	call CHECK2

	; a = (uint8_t)hl
	ld a,l
	add a,(iy+18)

	bit 7,(iy+5)
	jr nz,AFTER_FLIP_CHECK_Y ; skip these lines if F_rayDirX is negative

	neg
	dec a

AFTER_FLIP_CHECK_Y:
	; convert 0-255 range to 0-63 range
	and a,252
	ld d,a
	ld e,16
	mlt de

	ld ix,(_texture_pointer)
	add ix,de
	ld (_texture_pointer),ix

	pop hl

	pop ix
    ret

SIDEDISTXBIGGER:
	exx

	add hl,de ; F_sideDistY += F_deltaDistY
	add ix,bc ; map pointer += stepY

	; Unswap registers
	exx

	; Check if we hit a wall
	ld a,(ix)
	or a,a
	jr z,RAYLOOP
	;jr ENDOFLOOP_X

ENDOFLOOP_X:
	; hl -= de
	exx       ; swap
	sbc hl,de

	; bc = hl (F_perpWallDist)
	ld bc,0
	ld a,c ; a = 0 (for multiplication later...)
	ld b,h
	ld c,l

	push hl

	; de = F_rayDirX
	ld de,(iy+3)

	; hl = bc*de
	call CHECK2
	ld a,l
	add a,(iy+15) ; a += posX

	bit 7,(iy+8)
	jr z,AFTER_FLIP_CHECK_Y ; skip these lines if F_rayDirY is negative

	neg
	dec a

AFTER_FLIP_CHECK_X:
	; convert 0-255 range to 0-63 range
	and a,252
	ld d,a
	ld e,16
	mlt de

	ld ix,(_texture_pointer)
	add ix,de
	ld (_texture_pointer),ix

	pop hl

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
