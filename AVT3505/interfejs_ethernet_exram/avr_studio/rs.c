#include <avr/io.h>
#include "rs.h"

int rs_put(char znak)
{
	//Oczekiwanie a¿ bufor nadajnika jest pusty
	while(!(1<<UDRE0 & UCSR0A)) {}
	UDR0 = znak;
	return 0;
}

int rs_get(void)
{
	char znak;
	//Oczekiwanie na pojawienie siê danej
	while(!(1<<RXC0 & UCSR0A)) {}
	znak = UDR0;
	return znak;
}
