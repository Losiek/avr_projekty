#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>

#include "rs.h"
#include "harddef.h"
#include "fcheat_stdio.h"

fcheat_file fileRS = FCHEAT_STATIC_FDEVOPENWR(rs_put, rs_get);
FILE* fRS = (void*)&fileRS;

int main(void)
{
	uint8_t u8a, u8b;
	uint16_t u16a, u16b, u16c;
	uint32_t u32a;
	///////////////////////////////////////////////
	// Inicjacja portu szeregowego
	RS_SET_BAUD(DEF_BAUD); 
	UCSR0C = 1<<URSEL0 | 1<<UCSZ01 | 1<<UCSZ00; 
	UCSR0B = 1<<RXEN0 | 1<<TXEN0; 
	UCSR0A = 0;
	// Koniec inicjacji
	///////////////////////////////////////////////
	// Inicjacja funkcji dla domyœlnych strumieni we/wy
	//fRS = fdevopen(rs_put, rs_get, 0);
	
	fputs_P(PSTR("Hello world!\r\n"), fRS);
	
	u8a = 100;
	u8b = 200;
	u8a = (u8b * u8a) / 100;
	fprintf_P(fRS, PSTR("8x8 Wynik %u\r\n"),(unsigned int)u8a);
	
	u16a = 200;
	u16b = 600;
	u16a = (u16b * u16a) / 100;
	fprintf_P(fRS, PSTR("16x16 Wynik %u\r\n"),(unsigned int)u16c);
	
	u16c = (uint32_t)(u16a * u16b) / 10;
	fprintf_P(fRS, PSTR("(32)(16x16) Wynik: %u\r\n"), (unsigned int)u16c);
	
	u16c = ((uint32_t)u16a * (uint32_t)u16b) / 10;
	fprintf_P(fRS, PSTR("(32)16x(32)16 Wynik: %u\r\n"), (unsigned int)u16c);
	
	u16c = ((uint32_t)u16a * u16b) / 10;
	fprintf_P(fRS, PSTR("(32)16x16 Wynik: %u\r\n"), (unsigned int)u16c);
	
	u32a = u16a * u16b;
	fprintf_P(fRS, PSTR("32=16x16 Wynik: %lu\r\n"), (unsigned long int)u32a);
	
	u32a = (uint32_t)u16a * u16b;
	fprintf_P(fRS, PSTR("32=(32)16x16 Wynik: %lu\r\n"), (unsigned long int)u32a);

	fputs_P(PSTR("Koniec\r\n"), fRS);
}
