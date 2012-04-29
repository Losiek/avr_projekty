#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>

#include "rs.h"
#include "harddef.h"

#define EXMEM __attribute__ ((section (".exram")))

//funkcja wykonywana przed funkcj¹ main()
void before_main(void) __attribute__((naked)) __attribute__((section(".init3")));

void before_main(void) 
{
	MCUCR = 1<<SRE;
	SFIOR = 1<<XMBK | 1<<XMM0 /*A15*/;
	DDRC = 0x80;
	PORTC &= ~(1<<7);
}
//inicjacja zmiennej w exram o wielkoœci 1528 bajtów
uint8_t ex[1518] EXMEM;


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
	
	//zape³ninie komórek zmiennenj z pamiêci zew
	for(uint16_t i=0; i<1518; i++) 
	{
		ex[i] = 0xff;
	}
	
	//odczyt 10 pierwszych zape³nionych komórek
	for(uint16_t i=0; i<1518; i++)
	{
		printf("%x", ex[i]);
		printf("\r\n");
	}
}

