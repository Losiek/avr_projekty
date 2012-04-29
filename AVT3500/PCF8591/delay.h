#ifndef DELAY_H_INCLUDED
#define DELAY_H_INCLUDED

#define delay250ns() {asm volatile("nop"::);}
#define delay500ns()\
	{asm volatile( \
		"rjmp exit%=\n\t"\
		"exit%=:\n\t"::);}

#define delayus8(t)\
	{asm volatile( \
		"delayus8_loop%=: \n\t"\
		"nop \n\t"\
		"dec %[ticks] \n\t"\
		"brne delayus8_loop%= \n\t"\
	: :[ticks]"r"(t) );}
	// DEC - 1 cykl, BRNE 2 cykle, + 1xnop. Zegar 4MHz

// Deklaracja funkcji o najd³u¿szym opóŸnieniu
void delay100us8(uint8_t t);

#endif //DELAY_H_INCLUDED

