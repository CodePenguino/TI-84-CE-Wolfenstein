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

  section .text

; --------------------------------------------------------------------------------------------

	public __gfx_SetPixel2_NoClip
__gfx_SetPixel2_NoClip:
; Sets the color pixel to the global color index
; Arguments:
;  arg0 : X coordinate
;  arg1 : Y coordinate
; Returns:
;  None
	ld	hl,3
	add	hl,sp
	ld	bc,(hl)			; bc = x coordinate
	ld	de,3
	add	hl,de			; move to next argument
	ld	e,(hl)			; e = y coordinate
	add hl,de
	ld iy, (hl)     ; iy = color
;_SetPixel_NoClip_NoWait:
	ld	hl,(CurrentBuffer)
	add	hl,bc
	ld	d,ti.lcdWidth / 2
	mlt	de
	add	hl,de
	add	hl,de
	; ---- jank ----
	;ld iy, 257
	; ---- jank ----

	ld	(hl),iy			; get the actual pixel
;smcByte _Color
	ret

; --------------------------------------------------------------------------------------------