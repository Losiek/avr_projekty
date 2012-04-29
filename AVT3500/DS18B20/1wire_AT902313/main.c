#include <avr\io.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "harddef.h"
#include "1wire.h"


int main(void) {
	
	//Konfiguracja portów.
	LEDDDR = 0xff;
	DDRD  &= ~(1<<wire);
	PORTD &= ~(1<<wire);
	//koniec konfiguracji portów.
	//////////////////////////////////////////////////////
	wait_1w(200);
	reset_1w();


	for(;;) {
	    if (reset_1w()) {
	    	PORTB |= ~(1<<LED_A);
		} else {
			PORTB &= (1<<LED_A);
		}
	}
return 0;
}





	


