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
    ;    hl = fxmul8((uint8_t)posX, F_deltaDistX)
    ;    hl = F_sideDistX
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


    ;; ---------------------------------------------
    ;;    hl = F_sideDistY
    ;exx

    ;ld a,(iy+21)
    ;ld de,(iy+15)

    ;ld h,a
    ;ld l,e
    ;mlt hl

	;; hl /= 256
	;ld l,h
	;ld h,0

	;; bc = a*d (whole)
    ;ld b,a
    ;ld c,d
    ;mlt bc

	;add hl,bc

    ;exx
    ;; ---------------------------------------------

    ret
