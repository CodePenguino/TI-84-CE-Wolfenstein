	assume adl=1

section .rodata

	public _lcd_settings
_lcd_settings:
	; LcdTiming0
	db $60,$01,$0E,$0F ; PPL=400, HSW=2, HBP=16, HFP=15 (total=433)
	; LcdTiming1
	db $5F,$00,$FF,$70 ; LPP=96, VSW=1, VBP=112, VFP=255 (total=464)
	; LcdTiming2
	db $00,$78,$8F,$01 ; PCD=2, CPL=400

section .text

include 'ti84pceg.inc'

CurrentBuffer      := ti.mpLcdLpbase

macro SPI_CMD cmd
	ld a,cmd
	call spiCmd
end macro

macro SPI_PARAM param
	ld a,param
	call spiParam
end macro

;macro SPI_PARAM16 param
	;ld a, param >> 8
	;call spiParam

	;ld a, param & $FF
	;call spiParam
;end macro

	public _set_scaled_mode
_set_scaled_mode:
	SPI_CMD $2A      ; X left/right bounds
	SPI_PARAM 0
	SPI_PARAM 0
	SPI_PARAM 0
	SPI_PARAM 159

	SPI_CMD $2B      ; Y up/down bounds
	SPI_PARAM 0
	SPI_PARAM 0
	SPI_PARAM 0
	SPI_PARAM 239

	SPI_CMD $B0      ; RAM control vsync
	SPI_PARAM $12
	SPI_PARAM $F0

	SPI_CMD $3A      ; RGB 16bpp
	SPI_PARAM $56

	SPI_CMD $33      ; Vertical scroll parameters
	SPI_PARAM 0		 ; Top fixed area
	SPI_PARAM 160
	SPI_PARAM 0		 ; Scrolling area
	SPI_PARAM 160
	SPI_PARAM 0      ; Bottom fixed area
	SPI_PARAM 0

	SPI_CMD $37		 ; Vertical scroll start address
	SPI_PARAM 0

	SPI_CMD $E4      ; Gate control
	SPI_PARAM $27    ; 320 lines
	SPI_PARAM $00    ; Start line 0
	SPI_PARAM $14    ; No interlacing

	ret

; All this code copy-pasted from this: https://wikiti.brandonw.net/index.php?title=84PCE:Ports:D000
; Input: A = parameter
spiParam:
	scf ; First bit is set for data
	db 030h ; jr nc,? ; skips over one byte
	; Input: A = command
spiCmd:
	or a,a ; First bit is clear for commands
	ld hl,0F80818h
	call spiWrite
	ld l,h
	ld (hl),001h
spiWait:
	ld l,00Dh
spiWait1:
	ld a,(hl)
	and a,0F0h
	jr nz,spiWait1
	dec l
spiWait2:
	bit 2,(hl)
	jr nz,spiWait2
	ld l,h
	ld (hl),a
	ret
spiWrite:
	ld b,3
spiWriteLoop:
	rla
	rla
	rla
	ld (hl),a ; send 3 bits
	djnz spiWriteLoop
	ret
