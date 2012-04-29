#include <avr/io.h>
#include <delay.h>
#include <inttypes.h>
#include "harddef.h"

int main(void) {
	DDRD = 0xff;
	DDRB = 0xff;
	
	PORTB = 0x00;
	
	return 0;
	}