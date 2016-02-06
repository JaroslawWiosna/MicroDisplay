;-----------------------------------------------------------------------------;
;
;-----------------------------------------------------------------------------;
;
;-----------------------------------------------------------------------------;


.nolist
#define ASM_TEMPLATE_ASM
#include "asm_template.h"
#include <avr/io.h>
.list

;----------------------------------------------------------------------------;
.global asm_func_01
.func asm_func_01
asm_func_01:
	ldi r18, 0xFF
	in r19, PORTB
	eor r19, r18
	out PORTB, r19
;	ret
.endfunc

.global togglePORTB
.func togglePORTB
togglePORTB:
	in	r25, 0x05	; 0x05 = PORTB
	ldi	r24, 0xFF	; 1
	eor	r24, r25
	out	0x05, r24	; 5
	ret
.endfunc




