#include <avr\io.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "harddef.h"
#include "1wire_2.h"


int main(void) {
	
	//Konfiguracja portów.
	
	//koniec konfiguracji portów.
	//////////////////////////////////////////////////////
   RESET_PULSE();
   send_byte(0xAA);
   uint8_t k;
	for(;;) {
	
		}
return 0;
}





	




