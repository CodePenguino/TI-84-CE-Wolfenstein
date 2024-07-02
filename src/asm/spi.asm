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

; macro SPI_PARAM param
; 	db param
; end macro

; macro SPI_PARAM16 param
; 	SPI_PARAM((param >> 8))
; 	SPI_PARAM((param & $FF))
; end macro

; macro SPI_PARAM param
; 	db ((param * SPI_BIT) >> 8) | (SPI_VALUE | SPI_BIT)
; 	define SPI_VALUE eval((param * SPI_BIT) & $FF)
; 	if SPI_BIT == $01
; 		SPI_END
; 		SPI_START
; 	else
; 		define SPI_BIT eval(SPI_BIT >> 1)
; 	endif
; end macro

; macro SPI_PARAM16 param
; 	SPI_PARAM(param >> 8)
; 	SPI_PARAM(param & $FF)
; end macro

; macro SPI_START
; 	define SPI_BIT $80
; 	define SPI_VALUE $00
; end macro

; ; macro SPI_CMD cmd 
; ; 	db ((cmd * SPI_BIT) >> 8) | SPI_VALUE
; ; 	SPI_VALUE := ((cmd * SPI_BIT) & $FF)
; ; 	if SPI_BIT == $01
; ; 		SPI_END
; ; 		SPI_START
; ; 	else
; ; 		SPI_BIT := (SPI_BIT >> 1)
; ; 	endif
; ; end macro

; macro SPI_CMD cmd
; 	db ((cmd * SPI_BIT) >> 8) | SPI_VALUE	
; 	define SPI_VALUE eval((cmd * SPI_BIT) & $FF)
; 	if SPI_BIT == $01
; 		SPI_END
; 		SPI_START
; 	else
; 		define SPI_BIT eval(SPI_BIT >> 1)
; 	endif
; end macro

; macro SPI_END
; 	if SPI_BIT != $80
; 	db SPI_VALUE
; 	endif
; 	undefine SPI_VALUE
; 	undefine SPI_BIT
; end macro
; -----------------------------  Fuck you ---------------------------------

	public _set_scaled_mode
_set_scaled_mode:
	ld de,spiResetWindowAddress
	ld b,spiDisableRamAccessSize
	call spiFastTransfer

	; APTR(lcdSettings8BitStretched)
	; ACALL(setLcdSettingsFirstBuffer)
	
	ret

; Functions...
spiFastTransfer:
	ld hl,$F80008	; mpSpiTransfer
	ld (hl),1		; Activate the SPI chip select signal
	; Fill SPI FIFO and transfer at the same time
	ld l,$18		; mpSpiFifo & $FF
taxEvasion:
	ld a,(de)
	ld (hl),a
	inc de
	djnz taxEvasion
	; Wait for transfer to complete
	ld l,$0C		; mpSpiStatus & $FF
taxEvasion2:		; Electric boogaloo!
	bit 2,(hl)
	jr nz,taxEvasion2
	; Disable transfer, discarding excess bits sent to LCD
	ld l,$08		;mpSpiTransfer & $FF
	ld (hl),b
	ret


; Arrays...
spiResetWindowAddress:
	db 88	; SPI_CMD(0xb0)
	db 64	; SPI_PARAM(0x2)
	db 190	; SPI_PARAM(0xf0)
spiDisableRamAccessSize = $+1 - spiResetWindowAddress
	db 2	; SPI_CMD(0x2c)
	db 197	; SPI_CMD(0xb0)
	db 132	; SPI_PARAM(0x12)
	db 75	; SPI_PARAM(0xf0)
	db 224	; SPI_END
spiResetWindowAddressSize = $ - spiResetWindowAddress
