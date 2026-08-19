	.assume adl=1
	.include "src/asm/vline.asm"

CurrentBuffer      := 0E30014h
ScreenWidth        := 160

; The assembler couldn't handle a 180-repeat loop
; so the code had to be duplicated...
.macro texmac from=1, to=60
	exx
	add hl,bc
	ld  e,h
	ld  a,(de)
	exx

	ld (hl),a
	add hl,de
	ld (hl),a
	add hl,de
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

	.section .text.__gfx_TexturedVertLine_Partial2
	.global __gfx_TexturedVertLine_Partial2
	.type __gfx_TexturedVertLine_Partial2, @function
__gfx_TexturedVertLine_Partial2:
	ld  hl,(CurrentBuffer)
	; hl += x
	ld  a,(_RENDER_x)
	ld  l,a

    ld.sis bc,(_RENDER_length)       ; bc = other_length
	res 0,c                          ; make sure other_length is even

	ld  iy,drawVertLine
	add iy,bc

	push iy

	ld a,0xEC                        ; sets ceiling color

	ld  de,ScreenWidth

	call __gfx_VertLine_NoClip

	ld iy,PARTIAL_JUMPTABLE
	add iy,bc
	add iy,bc

    ld  bc,(_texture_pointer)        ; bc = texture pointer

	ld  a,(bc)

	call __gfx_VertLine_NoClip

	pop iy

	ld a,0x19                        ; sets floor color

	jp (iy)

PARTIAL_JUMPTABLE:
	jp vline_0
	jp vline_2
	jp vline_4
	jp vline_6
	jp vline_8
	jp vline_10
	jp vline_12
	jp vline_14
	jp vline_16
	jp vline_18
	jp vline_20
	jp vline_22
	jp vline_24
	jp vline_26
	jp vline_28
	jp vline_30
	jp vline_32
	jp vline_34
	jp vline_36
	jp vline_38
	jp vline_40
	jp vline_42
	jp vline_44
	jp vline_46
	jp vline_48
	jp vline_50
	jp vline_52
	jp vline_54
	jp vline_56
	jp vline_58
	jp vline_60
	jp vline_62
	jp vline_64
	jp vline_66
	jp vline_68
	jp vline_70
	jp vline_72
	jp vline_74
	jp vline_76
	jp vline_78
	jp vline_80
	jp vline_82
	jp vline_84
	jp vline_86
	jp vline_88
	jp vline_90
	jp vline_92
	jp vline_94
	jp vline_96
	jp vline_98
	jp vline_100
	jp vline_102
	jp vline_104
	jp vline_106
	jp vline_108
	jp vline_110
	jp vline_112
	jp vline_114
	jp vline_116
	jp vline_118
	jp vline_120
	jp vline_122
	jp vline_124
	jp vline_126
	jp vline_128
	jp vline_130
	jp vline_132
	jp vline_134
	jp vline_136
	jp vline_138
	jp vline_140
	jp vline_142
	jp vline_144
	jp vline_146
	jp vline_148
	jp vline_150
	jp vline_152
	jp vline_154
	jp vline_156
	jp vline_158
	jp vline_160
	jp vline_162
	jp vline_164
	jp vline_166
	jp vline_168
	jp vline_170
	jp vline_172
	jp vline_174
	jp vline_176
	jp vline_178

drawVertTex:
	texmac
	ret

	.section .text.__gfx_TexturedVertLine_Full
	.global __gfx_TexturedVertLine_Full
	.type __gfx_TexturedVertLine_Full, @function

__gfx_TexturedVertLine_Full:
	ld  hl,(CurrentBuffer)             ; Set hl to current video buffer
    ld  a,(_RENDER_x)
    ld  l,a

	exx
    ld.sis hl,(_RENDER_texture_offset) ; hl' = texture offset (in fixed point)
	ld  de,(_texture_pointer)          ; de' = texture pointer
    ld.sis bc,(_RENDER_delta)          ; bc' = texture delta

	; I wish you could do "add de,h" but instead I need to do this
	ld  a,e
	add a,h
	ld  e,a

	ld  h,e
	exx

	ld  de,ScreenWidth                 ; de = screen width

	jp drawVertTex

	;texmac
	;ret


	; Draws a colored vertical line
__gfx_VertLine_NoClip:

	jp (iy)

	; Same unrolled loop trick as used for the textured line
drawVertLine:
	colmac
	ret
