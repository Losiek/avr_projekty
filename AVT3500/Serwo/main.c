//Sterowanie Serwomechanizmem
//PWM Input range: Pulse Cycle 20+-2ms, Positive Pulse 1~2ms
//Speed: 0.1s/2,094rad (60)

#include <avr\io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include "harddef.h"
#include "serwo.c"

int main(void) {
	////////////////////////////////////////
	// inicjacja
	DDRB = _BV(2);
	PORTB = ~(_BV(2));
	
	SWDDR = ~(1<<SW1 | 1<<SW2);
	SWPORT = 1<<SW1 | 1<<SW2;
	
	// koniec inicjacji
	///////////////////////////////////////
	
	uint8_t b = 50;
	for(;;) {
		
		serwo(150, 0);
		
		if(!(SWPIN & 1<<SW1))
		{
			// usuwanie drgañ styków
			_delay_ms(50);
			while(!(SWPIN & 1<<SW1))
			{
				serwo(250, 0);
			}
		}
		
		if(!(SWPIN & 1<<SW2))
		{
			// usuwanie drgañ styków
			_delay_ms(50);
			while(!(SWPIN & 1<<SW2))
			{
				serwo(0, 0);
			}
		}
		}
	return 0;
}	
