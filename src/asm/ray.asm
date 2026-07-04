    assume adl=1

    section .text

macro MUL8CODE
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
end macro


    ; F_rayDirX is negative
SIDEDISTMUL_NEG:
    ; (-posX)-1
    neg
    dec a

    MUL8CODE ; result stored in hl

    ; e = stepX
    ld e,1
    ret

SIDEDISTMUL_POS:
    MUL8CODE

    ; e = stepY
    ld e,-1
    ret


public _raycast
_raycast:
    ld iy,0
    add iy,sp

    ; ---------- F_sideDistX ----------

    ; a = (uint8_t)posX
    ld a,(iy+18)

    ; bc = F_deltaDistX
    ld bc,(iy+12)

    ; de = F_rayDirX
    ld de,(iy+6)

    ; hl = F_sideDistX
    ; d = stepX
    bit 7,d
    call z,SIDEDISTMUL_POS ; F_rayDirX is positive
    call nz,SIDEDISTMUL_NEG ; F_rayDirX is negative

    ; e = mapX
    ld e,(iy+19)

    ; ---------- F_sideDistY ----------
    exx

    ; a = (uint8_t)posY
    ld a,(iy+21)

    ; bc' = F_deltaDistY
    ld bc,(iy+15)

    ; de' = F_rayDirY
    ld de,(iy+9)

    ; hl' = F_sideDistY
    ; d' = stepY
    bit 7,d
    call z,SIDEDISTMUL_POS ; F_rayDirY is positive
    call nz,SIDEDISTMUL_NEG ; F_rayDirY is negative

    ; e' = mapY
    ld e,(iy+22)

    ;ld a,h

    exx
    ; ---------------------------------

LOOP:

    ; F_sideDistY > F_sideDistX
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

SIDEDISTYBIGGER:
    add hl,bc ; F_sideDistX += F_deltaDistX

    ; mapX += stepX
    ld a,e ; a = mapX
    add a,d
    ld e,a

    ld a,d
    ld d,0

    push hl
    ld hl,(_Map)
    add hl,de
    pop hl
    ld d,a


SIDEDISTXBIGGER:
    exx
    exx

    ret


    section .rodata
public _Map
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
