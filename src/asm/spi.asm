	assume adl=1
	section .rodata

include 'ti84pceg.inc'

; -----------------------------  Fuck you ---------------------------------
; Calls a routine located in the archived appvar.
; The 16-bit offset (plus 1) is stored at the return address.
ArcCall:
	ex (sp),hl
	ld (ArcCallSMC),hl
	inc hl
	inc hl
	ex (sp),hl
ArcCallEntry:
	push hl
	push de
ArcCallSMC = $+1
	ld hl,(0)
	dec.s hl
ArcBase = $+1
	ld de,0
	add hl,de
	pop de
	ex (sp),hl
	ret

; Puts a pointer located in the archived appvar in HL.
; The 16-bit offset (plus 1) is stored at the return address.
ArcPtr:
	pop hl
	ld e,(hl)
	inc hl
	ld d,(hl)
	inc hl
	push hl
	dec.s de
	ld hl,(ArcBase)
	add hl,de
	ret

macro ACALL address 
	call ArcCall
	dl address+1
end macro

macro APTR address
	call ArcPtr
	dl address+1
end macro

macro SPI_PARAM param
	db param
end macro

macro SPI_PARAM16 param
	SPI_PARAM((param >> 8))
	SPI_PARAM((param & $FF))
end macro

macro SPI_START
	define SPI_BIT $80
	define SPI_VALUE $00
end macro

macro SPI_CMD cmd 
	db ((cmd * SPI_BIT) >> 8) | SPI_VALUE
	SPI_VALUE := ((cmd * SPI_BIT) & $FF)
	if SPI_BIT == $01
		SPI_END
		SPI_START
	else
		SPI_BIT := (SPI_BIT >> 1)
	endif
end macro

macro SPI_END
;	if SPI_BIT != $80
;		db SPI_VALUE
;	endif
;	undef SPI_VALUE
;	undef SPI_BIT
end macro

; -----------------------------  Fuck you ---------------------------------

	public _set_scaled_mode
_set_scaled_mode:
	APTR(lcdSettings8BitStretched)
	ACALL(setLcdSettingsFirstBuffer)
	
	ret

setLcdSettingsFirstBuffer:
	ld de,0xD52C00 ; vRam + (320*240)

	; In: (HL) = timing (12 bytes)
	;     (HL+12) = LCD control (3 bytes)
	;     (HL+15) = offset to SPI settings
	;     DE = new LCD base address
SetLCDSettings:
	push hl
	
	; Reset interrupt mask
	ld a,8
	ld hl,$E3001C
	ld (hl),a
	
	; Wait for DMA completion
	ld l,mpLcdIcr & $FF
	ld (hl),a
	ld l,$E30024 & $FF
_
	tst a,(hl)
	jr z,-_
	ld l,mpLcdIcr & $FF
	ld (hl),a

	; Set DMA base address
	ex de,hl
	ld (mpLcdBase),hl

	pop hl
	
	; Set timing parameters
	ld e,mpLcdTiming0 & $FF
	ld bc,12
	ldir





lcdSettings8BitStretched:
	; LcdTiming0
	db $C4,$03,$1D,$1F ; PPL=800, HSW=4, HBP=32, HFP=30 (total=866)
	; LcdTiming1
	db $2F,$00,$B7,$00 ; LPP=48, VSW=1, VBP=0, VFP=183 (total=232)
	; LcdTiming2
	db $00,$78,$1F,$03 ; PCD=2, CPL=800
	; LcdCtrl
	dl $013C27
	; SPI settings
	dl spi_doublescale_settings+1


	
	public spi_doublescale_settings
_spi_doublescale_settings:
	SPI_START
	; 2A             ; Column address set
	;SPI_CMD($2A)    ; SPI = 128
	db 21

	;SPI_PARAM16(0)   ;  Left bound
	db 0
	db 0
	;SPI_PARAM16(159) ;  Right bound
	db 0
	db 159

	;; 2B             ; Row address set
	;SPI_CMD($2B)     ; SPI = 64
	db 10

	;SPI_PARAM16(0)   ;  Upper bound
	db 0
	db 0

	;SPI_PARAM16(239) ;  Lower bound
	db 0
	db 239

	;; B0             ; RAM Control
	;SPI_CMD($B0)     ; SPI = 32
	db 23

	;SPI_PARAM($12)   ;  VSYNC Interface
	db $12
	db $F0

	;; 33             ; Vertical scroll parameters
	;SPI_CMD($33)     ; SPI = 16
	db 3
	
	; Top fixed area
	db 0
	db 0

	; Scrolling area
	db 0
	db 160

	; Bottom fixed area
	db 0
	db 160

	;; 37             ; Vertical scroll amount
	;SPI_CMD($37)     ; SPI = 8
	db 1

	; Duplicate right side to left (320)
	db 1
	db 64

	;; E4             ; Gate control
	;SPI_CMD($E4)     ; SPI = 4
	db 3
	
	; 320 lines
	db $27

	; Start line 0
	db $00

	; Interlace
	db $14

	; SPI_END
	db $00
