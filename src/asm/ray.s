    .assume adl=1

	.include "src/asm/fixed.asm"

MapSize := 24

; Ensure that carry = 0 before running...
.macro MUL24CODE_FRACTIONAL_ONLY
	; Store original copy of de in af and af'
	ld a,e
	ex af,af'
	ld a,d
	; ------ Whole bit (ignored for this...) ------
	sbc hl,hl

	; de = b*e
	ld d,b
	mlt de

	add hl,de ; add to result

	; ------ Fractional bit of a ------
	;ex af,af'
	ld e,a

	; de = c*d (whole)
	ld d,c
	mlt de

	add hl,de ; add to result

	; load e
	ex af,af'
	ld e,a

	; de = c*e (fractional)
	ld d,c
	mlt de
	; de /= 256
	ld e,d
	ld d,0 ; d = 0
	add hl,de ; add to result
.endm

    ; F_rayDirX is negative
SIDEDISTXMUL_NEG:
    MUL8CODE ; result stored in hl (F_sideDistX)

    ; bc = stepX
    ld bc,-MapSize
    ret

    ; F_rayDirX is positive
SIDEDISTXMUL_POS:
    ; (-pos)-1
	cpl

    MUL8CODE

    ; bc = stepX
    ld bc,MapSize
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
	cpl

    MUL8CODE

    ; bc' = stepY
    ld bc,1
    ret

; ------------------------------------------------


	.section .text._raycast
	.global _raycast
	.type _raycast, @function
_raycast:
    ;ld iy,0
    ;add iy,sp
	push ix

	; Setup for map pointer (ix)
	ld ix,_Map

    ; ---------- F_sideDistY setup ----------
    ; de' = mapY (posY high byte)
    ; de = 0
    ld de,0

    ;ld e,(iy+19)
    ld a,(_posY+1)
    ld e,a
	add ix,de               ; offset map pointer by mapY

    ; de' = F_deltaDistY
    ld.sis de,(_F_deltaDistY)

    ; bc' = F_rayDirY
    ld a,(_F_rayDirY+1)

	or a,a                  ; set sign flag, carry = 0
    ; a' = (uint8_t)posY
    ld a,(_posY)

    ; hl' = F_sideDistY
    ; bc' = stepY
    call p,SIDEDISTYMUL_POS ; F_rayDirY is positive
    call m,SIDEDISTYMUL_NEG ; F_rayDirY is negative

    exx

    ; ---------- F_sideDistX setup ----------
    ; de = mapX (posX high byte)
    ld a,(_posX+1)
    ld d,a
	ld e,MapSize
	mlt de
	add ix,de               ; offset map pointer by mapX*MapSize

    ; de = F_deltaDistX
    ld.sis de,(_F_deltaDistX)

    ; a = F_rayDirX (high byte)
    ld a,(_F_rayDirX+1)

	or a,a                  ; set sign flag, carry = 0
    ; a = (uint8_t)posX
    ld a,(_posX)

    ; hl = F_sideDistX
    ; bc = stepX
    call p,SIDEDISTXMUL_POS ; F_rayDirX is positive
    call m,SIDEDISTXMUL_NEG ; F_rayDirX is negative

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
    sub a,a      ; a = 0
    or a,(ix)    ; set z flag... (and carry = 0)
	jr z,RAYLOOP ; not hit a wall? jump back to beginning
	;jr ENDOFLOOP_Y ; else, jump to end

ENDOFLOOP_Y:
	; hl -= de
	sbc hl,de

	; bc = hl (F_perpWallDist)
	ld b,h
	ld c,l

	push hl

	ld ix,(_texture_pointer)

	; offset by 8192 (64*64*2) * whatever value we hit (minus 1)
	; 8192 = 32*256...
	dec a
    rra
    rra
    rra
    rra
	ld d,a
	ld e,0
	add ix,de ; carry = 0

	; de = F_rayDirY
    ld.sis de,(_F_rayDirY)

	; hl = bc*de
	MUL24CODE_FRACTIONAL_ONLY

	; a = (uint8_t)hl
    ld a,(_posY)
    add a,l

    ld.sis hl,(_F_rayDirX)
    bit 7,h
	jr nz,AFTER_FLIP_CHECK_Y ; skip this line if F_rayDirX is negative

	cpl

AFTER_FLIP_CHECK_Y:
	; convert 0-255 range to 0-63 range
	and a,%11111100
	ld d,a
	ld e,16
	mlt de

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
    sub a,a      ; a = 0
    or a,(ix)    ; set z flag... (and carry = 0)
	jr z,RAYLOOP ; not hit a wall? jump back to beginning
	;jr ENDOFLOOP_X

ENDOFLOOP_X:
	; hl -= de
	exx       ; swap
	sbc hl,de

	; bc = hl (F_perpWallDist)
	ld b,h
	ld c,l

	push hl

	ld ix,(_texture_shadow_pointer)

	; offset by 8192 (64*64*2) * whatever value we hit (minus 1)
	; 8192 = 32*256...
	dec a
    rra
    rra
    rra
    rra
	ld d,a
	ld e,0
	add ix,de

	; de = F_rayDirX
	ld.sis de,(_F_rayDirX)

	; hl = bc*de
	MUL24CODE_FRACTIONAL_ONLY
    ld a,(_posX)
    add a,l

    ld.sis hl,(_F_rayDirY)
    bit 7,h
	jr z,AFTER_FLIP_CHECK_X ; skip this line if F_rayDirY is positive

	cpl

AFTER_FLIP_CHECK_X:
	; convert 0-255 range to 0-63 range
	and a,%11111100
	ld d,a
	ld e,16
	mlt de

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
