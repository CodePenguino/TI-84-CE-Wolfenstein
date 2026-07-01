assume adl=1
include 'ti84pceg.inc'

LcdSize            := ti.lcdWidth*ti.lcdHeight
CurrentBuffer      := ti.mpLcdLpbase

	section .text

	public _gfx_SetPixel2_NoClip
_gfx_SetPixel2_NoClip:
	; Set up iy register
	ld  iy, 0
	add iy,sp

	ld  hl,(CurrentBuffer) ; Set hl to current video buffer
	ld  bc,(iy+3)          ; bc = x
	add hl,bc              ; hl += bc
	ld  c,(iy+6)           ; c = y
	ld  b,ti.lcdWidth / 2  ; b = lcdWidth / 2 = 160
	mlt bc                 ; bc = b * c (y * 160)
	add hl,bc
	ld  a,(iy+9)           ; a = c
	ld	(hl),a             ; set the pixel color
	ret



	public __gfx_TexturedVertLine_Partial
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
	ld  c,e                ; c = other_length

	exx
	ld  de,(iy+12)         ; de' = texture pointer
	ld  h,e
	ld  l,0                ; hl' = texture pointer (fixed point)
	ld  bc,(iy+15)         ; bc' = delta (fixed point)
	exx

	ld  iy,drawVertLine
	add iy,de
	ld  de,ti.lcdWidth/2

	call __gfx_VertLine_NoClip

	pop de                 ; de = length

	ld  iy,drawVertTex
	add iy,de

	ld  de,ti.lcdWidth/2   ; de = screen width
	ld  b,0

	jp (iy)

drawVertTex:
repeat 180                 ; Kids, SERIOUSLY don't try this at home...
	exx
	add hl,bc
	ld  e,h
	ld  a,(de)
	exx

	ld (hl),a
	add hl,de
end repeat

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
	ld  de,ti.lcdWidth/2

	jp (iy)

	public __gfx_TexturedVertLine_Full
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

	ld  de,ti.lcdWidth/2   ; de = screen width
	ld  b,1                ; Used to return from drawVertTex early only if
                           ; we're jumping from this function
	jp drawVertTex

	; Draws a colored vertical line
	public __gfx_VertLine_NoClip
__gfx_VertLine_NoClip:
	jp (iy)

	; Same unrolled loop trick as used for the textured line
drawVertLine:
repeat 90
	ld (hl),a
	add hl,de
end repeat
	ret

	public __gfx_VertLine_Scuffed
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
	ld  de,ti.lcdWidth/2

	jp (iy)
	ret
