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
	UCSR0C = 1<<URSEL0 | 1<<UCSZ01 | 1<<UCSZ00;
	UCSR0B = 1<<RXEN0 | 1<<TXEN0;
	UCSR0A = 0;
	// Koniec inicjacji
	///////////////////////////////////////////////
	
	// Inicjacja funkcji dla domyœlnych strumieni we/wy
	fdevopen(rs_put, rs_get);
	uint8_t chain[19] = {"Termometr DS18B20!"};
	for(uint8_t i=0; i<19; i++) {
		rs_put(chain[i]);
	}
	
	RESET_PULSE();
	SKIP_ROM();
	READ_SCRATCHPAD();
	RESET_PULSE();
	
	char buf[6];
	
	for(;;) {
		dtostrf(TEMP_READ(),2,2,buf);
	
		for(uint8_t i=0; i<5; i++) {
			rs_put(buf[i]);
		}
		puts("\r");
	}
	
return 0;
}





	

