  assume adl=1
	include 'ti84pceg.inc'

LcdSize            := ti.lcdWidth*ti.lcdHeight
CurrentBuffer      := ti.mpLcdLpbase

macro mIsHLLessThanDE?
	or	a,a
	sbc	hl,de
	add	hl,hl
	jp	po,$+5
	ccf
end macro
macro mIsHLLessThanBC?
	or	a,a
	sbc	hl,bc
	add	hl,hl
	jp	po,$+5
	ccf
end macro
macro s8 op, imm
	local i
 	i = imm
	assert i >= -128 & i < 128
	op, i
end macro

;-------------------------------------------------------------------------------
wait_quick.usages_counter = 0

;postpone
	wait_quick.usages := wait_quick.usages_counter
;end postpone

macro wait_quick?
	call	_WaitQuick
	wait_quick.usages_counter = wait_quick.usages_counter + 1
end macro

;-------------------------------------------------------------------------------
macro setSmcBytes name*
	local addr, data
	postpone
		virtual at addr
			irpv each, name
				if % = 1
					db %%
				end if
				assert each >= addr + 1 + 2*%%
				dw each - $ - 2
			end irpv
			load data: $-$$ from $$
		end virtual
	end postpone

	call	_SetSmcBytes
addr	db	data
end macro

macro setSmcBytesFast name*
	local temp, list
	postpone
		temp equ each
		irpv each, name
			temp equ temp, each
		end irpv
		list equ temp
	end postpone

	pop	de			; de = return vetor
	ex	(sp),hl			; l = byte
	ld	a,l			; a = byte
	match expand, list
		iterate expand
			if % = 1
				ld	hl,each
				ld	c,(hl)
				ld	(hl),a
			else
				ld	(each),a
			end if
		end iterate
	end match
	ld	a,c			; a = old byte
	ex	de,hl			; hl = return vector
	jp	(hl)
end macro

macro smcByte name*, addr: $-1
	local link
	link := addr
	name equ link
end macro

; YOLO!!!
macro vertLoop i*
	repeat i
		_DrawVert#%:
			repeat %
				ld  (hl),bc
				add hl,de
			end repeat
			ret
	end repeat
end macro

; --------------------------------------------------------------------------------------------
  section .text

; Set up all 200 vertical line drawing functions
	vertLoop 200

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
	add hl,bc
	ld  de,(iy+9)          ; de = c
	ld	(hl),de            ; set the pixel color
	ret

	public _gfx_VertLine2_NoClip
_gfx_VertLine2_NoClip:
	; Set up iy register
	ld  iy, 0
	add iy,sp

	ld  hl,(CurrentBuffer) ; Set hl to current video buffer
	ld  bc,(iy+3)          ; bc = x
	add hl,bc              ; hl += bc
	ld  c,(iy+6)           ; c = y
	ld  b,ti.lcdWidth / 2  ; b = lcdWidth / 2 = 160
	mlt bc                 ; bc = b * c = y * 160
	add hl,bc
	add hl,bc
	
	ld  de,ti.lcdWidth
	ld  bc,(iy+12)         ; bc = c

	ld  iy,(iy+9)          ; iy = length

	; TODO: Replace this 200 with an actual number

	call _DrawVert1 + $000000
	;call _DrawVert1

; .loop:
; 	ld  (hl),iy
; 	add hl,de
; 	djnz .loop
	ret
