	.assume adl=1

CurrentBuffer      := 0E30014h
ScreenWidth        := 160

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
    ld  l,(iy+3)
	ld  c,(iy+6)           ; c = y
	ld  b,ScreenWidth      ; b = ScreenWidth
	mlt bc                 ; bc = b * c (y * 160)
	add hl,bc
	ld  a,(iy+9)           ; a = c
	ld	(hl),a             ; set the pixel color
	ret


	.section .text.__gfx_TexturedVertLine_Partial
	.global __gfx_TexturedVertLine_Partial
	.type __gfx_TexturedVertLine_Partial, @function

__gfx_TexturedVertLine_Partial:
	ld  hl,(CurrentBuffer)           ; Set hl to current video buffer
    ; hl += x
    ld  a,(_RENDER_x)
    ld  l,a

	ld  a,0xEC                       ; Sets ceiling color

    ld.sis bc,(_RENDER_other_length) ; bc = other_length
	res 0,c                          ; make sure other_length is even

	ld  iy,drawVertLine
	add iy,bc
    ld  de,ScreenWidth               ; de = ScreenWidth

	exx
    ld  de,(_texture_pointer)        ; de' = texture pointer
	ld  h,e
	ld  l,0                          ; hl' = texture pointer (fixed point)
	; (e must be zero if memory aligned...)
    ld.sis bc,(_RENDER_delta)       ; bc' = delta (fixed point)
	exx

	call __gfx_VertLine_NoClip

    ld.sis  de,(_RENDER_tex_length)

	ld  iy,drawVertTex
	add iy,de

	ld  de,ScreenWidth               ; de = screen width = 160
    xor a,a                          ; ensure that z = 0

	jp (iy)

drawVertTex:
	texmac

	; Used to check if we should return early or not...
	ret nz

drawFloor:
	ld  a,0x19                    ; Sets floor color

	ld  iy,drawVertLine
	add iy,bc

	jp (iy)

	.section .text.__gfx_TexturedVertLine_Full
	.global __gfx_TexturedVertLine_Full
	.type __gfx_TexturedVertLine_Full, @function

__gfx_TexturedVertLine_Full:
	ld  hl,(CurrentBuffer)          ; Set hl to current video buffer
    ld  a,(_RENDER_x)
    ld  l,a

	exx
    ld.sis  hl,(_RENDER_texture_offset) ; hl' = texture offset (in fixed point)
	ld  de,(_texture_pointer)       ; de' = texture pointer
    ld.sis  bc,(_RENDER_delta)          ; bc' = texture delta

	; I wish you could do "add de,h" but instead I need to do this
	ld  a,e
	add a,h
	ld  e,a

	ld  h,e
	exx

	ld  de,ScreenWidth              ; de = screen width

    or a,e                          ; ensure that z = nz
	jp drawVertTex

	; Draws a colored vertical line
__gfx_VertLine_NoClip:
	jp (iy)

	; Same unrolled loop trick as used for the textured line
drawVertLine:
	colmac
	ret
