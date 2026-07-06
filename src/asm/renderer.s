	.assume adl=1
;	.include "ti84pceg.inc"

CurrentBuffer      := 0E30014h

; The assembler couldn't handle a 180-repeat loop
; so the code had to be duplicated...
.macro texmac from=1, to=90
	exx
	add hl,bc
	ld  e,h
	ld  a,(de)
	exx

	ld (hl),a
	add hl,de

	exx
	add hl,bc
	ld  e,h
	ld  a,(de)
	exx

	ld (hl),a
	add hl,de

	.if \to-\from
	texmac "(\from+1)",\to
	.endif
.endm

.macro colmac from=1, to=90
	ld (hl),a
	add hl,de

	.if \to-\from
	colmac "(\from+1)",\to
	.endif
.endm

	.section .text._gfx_SetPixel2_NoClip
	.global _gfx_SetPixel2_NoClip
	.type _gfx_SetPixel2_NoClip, @function
_gfx_SetPixel2_NoClip:
	; Set up iy register
	ld  iy, 0
	add iy,sp

	ld  hl,(CurrentBuffer) ; Set hl to current video buffer
	ld  de,(iy+3)          ; de = x
	add hl,de              ; hl += de
	ld  c,(iy+6)           ; c = y
	ld  b,160              ; b = lcdWidth / 2 = 160
	mlt bc                 ; bc = b * c (y * 160)
	add hl,bc
	ld  a,(iy+9)           ; a = c
	ld	(hl),a             ; set the pixel color
	ret


	.section .text.__gfx_TexturedVertLine_Partial
	.global __gfx_TexturedVertLine_Partial
	.type __gfx_TexturedVertLine_Partial, @function

__gfx_TexturedVertLine_Partial:
	; Set up iy register
	ld  iy,0
	add iy,sp

	ld  hl,(CurrentBuffer) ; Set hl to current video buffer
	ld  de,(iy+3)          ; de = x
	add hl,de              ; hl += de

	ld  a,0xEC             ; Sets ceiling color

	ld  de,(iy+6)          ; de (in stack) = length
	push de

	ld  de,(iy+9)          ; de = other_length
	res 0,e                ; make sure other_length is even
	ld  c,e                ; c = other_length

	exx
	ld  de,(iy+12)         ; de' = texture pointer
	ld  h,e
	ld  l,0                ; hl' = texture pointer (fixed point)
	ld  bc,(iy+15)         ; bc' = delta (fixed point)
	exx

	ld  iy,drawVertLine
	add iy,de
	ld  de,160             ; de = lcdWidth / 2 = 160

	call __gfx_VertLine_NoClip

	pop de                 ; de = length

	ld  iy,drawVertTex
	add iy,de

	ld  de,160             ; de = screen width = 160
	ld  b,0

	jp (iy)

drawVertTex:
	texmac

	;; Used to check if we should return early or not...
	ld a,0
	cp a,b
	ret nz

drawFloor:
	ld  d,a
	ld  e,c                ; de = other_length

	ld  a,0x19             ; Set floor color

	ld  iy,drawVertLine
	add iy,de
	ld  de,160;ti.lcdWidth/2

	jp (iy)

	.section .text.__gfx_TexturedVertLine_Full
	.global __gfx_TexturedVertLine_Full
	.type __gfx_TexturedVertLine_Full, @function

__gfx_TexturedVertLine_Full:
	; Set up iy register
	ld  iy,0
	add iy,sp

	ld  hl,(CurrentBuffer) ; Set hl to current video buffer
	ld  bc,(iy+3)          ; bc = x
	add hl,bc              ; hl += bc

	exx
	ld  hl,(iy+9)          ; hl' = texture offset (in fixed point)
	ld  de,(iy+6)          ; de' = texture pointer
	ld  bc,(iy+12)         ; bc' = texture delta

	; I wish you could do "add de,h" but instead I need to do this
	ld  a,e
	add a,h
	ld  e,a

	ld  h,e
	exx

	ld  de,160;ti.lcdWidth/2   ; de = screen width
	ld  b,1                ; Used to return from drawVertTex early only if
                           ; we're jumping from this function
	jp drawVertTex

	; Draws a colored vertical line
__gfx_VertLine_NoClip:
	jp (iy)

	; Same unrolled loop trick as used for the textured line
drawVertLine:
	colmac
	ret

__gfx_VertLine_Scuffed:
	ld  iy,0
	add iy,sp

	ld  hl,(CurrentBuffer) ; Set hl to current video buffer
	ld  bc,(iy+3)          ; bc = x
	add hl,bc              ; hl += bc

	ld  de,(iy+6)          ; de = length
	ld  a,(iy+9)           ; Uses a byte duplication trick to draw two pixels at once

	ld  iy,drawVertLine
	add iy,de
	ld  de,160;ti.lcdWidth/2

	jp (iy)
	ret
