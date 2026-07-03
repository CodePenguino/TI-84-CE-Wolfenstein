    assume adl=1

    section .text

;macro

public _raycast
_raycast:
    ld iy,0
    add iy,sp

    ; a = (uint8_t)posX
    ld a,(iy+18)

    ; de = F_deltaDistX
    ld de,(iy+12)

    ; bc = F_rayDirX
    ld bc,(iy+6)

    ;bit 7,b ; F_rayDirX < 0?
    ;jr z,

SIDEDISTXMUL1:

    ret
