#include <avr\io.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "harddef.h"
#include "1wire.h"


int main(void) {
	
	//Konfiguracja portów.
	LEDDDR = 0xff;
	PORTB = 0xff;
	DDRD  &= ~(1<<wire);
	PORTD &= ~(1<<wire);
	//koniec konfiguracji portów.
	//////////////////////////////////////////////////////
	wait_1w(200);
	uint8_t is_there = 0;


	for(;;) {
		
	    if (reset_1w()==0) {
	    	PORTB &= ~(1<<LED_A);
		} 
		if (reset_1w()) {
	    	PORTB &= ~(1<<LED_B);
		} 
	}
return 0;
}





	


