
.include "atmega16.inc"

.equ    PORT,   PORTB
.equ    PIN,    2
.equ    DDR,    DDRB

.equ    CLOCK,  18432000

; ----------------------------------------------------------------------------
; This function sets up the hardware, any utilizes it.
; Sets up stack according to the hardware capabilities.
; ----------------------------------------------------------------------------

.global sysinit
.func   sysinit

        ldi     r16, (RAMSIZE-1) / 256
        out     SPH, r16
        ldi     r16, (RAMSIZE-1) % 256
        out     SPL, r16

; ----------------------------------------------------------------------------
; Setting up I/O ports: first of all, the Heart-beat LED
; 
; It is defined as PIN2 on PORTB port.
; ----------------------------------------------------------------------------
 
        sbi     DDR, PIN        ; DDR: bit value "1" means output

inf_loop:
        call    delay_500ms     ; doing some delay so that the blibking can be 
                                ; acceptable for human beings

        sbi     PORT, PIN       ; PORT: bit value "1" means HIGH value

        call    delay_500ms
        cbi     PORT, PIN

        rjmp    inf_loop

.endfunc

; ----------------------------------------------------------------------------
; Some delay: 500 ms
; 
; No prarameters needed.
; It provides 500 ms delay.
; ----------------------------------------------------------------------------

.func   delay_500ms
delay_500ms:
        push    r24
        push    r25

        ldi     r24, 500 % 256
        ldi     r25, 500 / 256
rloop:
        rcall   delay_1ms
        sbiw    r24, 1
        brne    rloop

        pop     r25
        pop     r24

        ret

.endfunc

; ----------------------------------------------------------------------------
; Some delay: 1 ms
; 
; No prarameters needed.
; It provides 1 ms delay.
; ----------------------------------------------------------------------------

.func   delay_1ms
delay_1ms:
        push    r24
        push    r25

        ldi     r24, (CLOCK/1000/4) % 256
        ldi     r25, (CLOCK/1000/4) / 256

dloop:
        sbiw    r24, 1      ; 2 cycle
        brne    dloop       ; 2 cycle

        pop     r25
        pop     r24

        ret

.endfunc

; ----------------------------------------------------------------------------

.text
.org 16384-1024
.global string_tab
string_tab:
.string "Hello, it is an AVR program."

