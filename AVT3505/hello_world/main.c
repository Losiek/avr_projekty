#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>

#include "rs.h"
#include "harddef.h"

int main(void)
{
	int a = 1234;
	int b = 0xff;
	///////////////////////////////////////////////////
	//Inicjacja portu szeregowego
	RS_SET_BAUD(DEF_BAUD);
	UCSR0C = 1<<URSEL0 | 1<<UCSZ01 | 1<<UCSZ00;
	UCSR0B = 1<<RXEN0 | 1<<TXEN0;
	UCSR0A = 0;
	//Koniec inicjacji
	//////////////////////////////////////////////////
	
	//inicjacja funkcji domyœlnych strumieni
	fdevopen(rs_put, rs_get);
	
	printf_P(PSTR("a(%%d)=%d\r\n"
				"a(%%x)=%x\r\n"
				"a(%%X)=%X\r\n"
				"b(%%#x)=%#x\r\n"
				"b(%%0)=%0\r\n"),
				a, a, a, b, b);
}

