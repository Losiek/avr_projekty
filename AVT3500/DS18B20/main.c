#include <avr\io.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <util/delay.h>
#include "harddef.h"
#include "1wire_2.h"
#include "rs.h"


int main(void) {
	
	//Konfiguracja portów.
	DDRB = 0;
	PORTB = 0;
	//Na sta³e dla magistrali 1-Wire.
	//PORTD &= ~(1<<wire);
	//koniec konfiguracji portów.
	//////////////////////////////////////////////////////
	//inicjacja portu szeregowego.
	RS_SET_BAUD(DEF_BAUD);
	UCSRB = (1<<RXEN) | (1<<TXEN);
	UCSRC = (3<<UCSZ0);
	// Koniec inicjacji
	///////////////////////////////////////////////
	
	// Inicjacja funkcji dla domyœlnych strumieni we/wy
	//fdevopen(rs_put, rs_get);
	uint8_t chain[19] = {"Termometr DS18B20!"};
	for(uint8_t i=0; i<19; i++) {
		rs_put(chain[i]);
	}
	
	RESET_PULSE();
	SKIP_ROM();
	READ_SCRATCHPAD();
	RESET_PULSE();
	
	for(uint8_t i=0; i<8; i++) {
		rs_put(bufor1w[i]);
	}
	
	
	//dtostrf(temp,1,1,buf);
	
	
	for(;;) {
		rs_put(TEMP_READ());
		
		
		}
	
return 0;
}





	

