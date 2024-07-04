	assume adl=1
	section .rodata

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

	SPI_CMD $3A      ; RGB 16bpp
	SPI_PARAM $56

	SPI_CMD $33      ; Vertical scroll parameters
	SPI_PARAM 0
	SPI_PARAM 160    ;  Top fixed area
	SPI_PARAM 0
	SPI_PARAM 160    ;  Scrolling area
	SPI_PARAM 0      ;  Bottom fixed area
	SPI_PARAM 0

	SPI_CMD $E4      ; Gate control
	SPI_PARAM $27    ; 320 lines
	SPI_PARAM $00    ; Start line 0
	SPI_PARAM $14    ; Interlace
	SPI_CMD $C6      ; Frame rate control
	SPI_PARAM 9      ; 394 clocks per line
	SPI_CMD $B2      ; Porch control
	SPI_PARAM 87     ; Back porch
	SPI_PARAM 1      ; Front porch

	; Set lcd settings...
	call lcdWriteTimings

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

; If you just stretch the screen with the SPI, it leaves gaps for every
; vertical scan lines. Setting the lcd to another mode fixes this...
lcdWriteTimings:
	ld hl,lcdSettings8BitStretched ; hl = lcd setting
	ld de,$E30000                  ; de = lcd_Timing0 memory address
	ld bc,12                       ; bc = number of bites in the setting
	ldir
	ret

; Arrays...
lcdSettings8BitStretched:
	; LcdTiming0
	db $C4,$03,$1D,$1F ; PPL=800, HSW=4, HBP=32, HFP=30 (total=866)
	; LcdTiming1
	db $2F,$00,$B7,$00 ; LPP=48, VSW=1, VBP=0, VFP=183 (total=232)
	; LcdTiming2
	db $00,$78,$1F,$03 ; PCD=2, CPL=800

lcdSettingsDefault:
	; LcdTiming0
	db $1F,$0A,$03,$38
	; LcdTiming1
	db $04,$02,$09,$3F
	; LcdTiming2
	db $00,$EF,$78,$02
