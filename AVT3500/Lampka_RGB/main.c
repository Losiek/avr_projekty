//Sterowanie PWM diodami LED wsp�la katoda

#include <avr\io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "harddef.h"

int main(void) {
	////////////////////////////////////////
	// inicjacja
	
	RGBDDR = 1<<Lgreen | 1<<Lblue | 1<<Lred;
	RGBPort = ~(1<<Lgreen | 1<<Lblue | 1<<Lred);
	
	TCCR0A = 1<<COM0A1 | 1<<COM0A0 | 1<<COM0B1 | 1<<COM0B0 | 1<<WGM01 | 1<<WGM00;
	TCCR0B = 1<<CS01 | 1<<CS00; 
	
	TCCR1A = 1<<COM1A1 | 1<<COM1A0 | 1<<COM1B1 | 1<<COM1B0 | 1<<WGM10;
	TCCR1B = 1<<CS11 | 1<<CS10; 
	
	TIMSK = 1<<TOIE1 | 1<<TOIE0;
	SREG = 1<<7;
	// koniec inicjacji
	///////////////////////////////////////
	
	red = 0;
	green = 0;
	blue = 0;
	
	uint8_t i;
	for(;;) {
		++red;
		_delay_ms(0x20);
	}		
	return 0;
}	
