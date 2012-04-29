#include <avr\io.h>
#include "rs.h"

int rs_put(char znak)
{
	// Oczekiwanie a¿ bufor nadajnika jest pusty
	while(!(UCSRA & 1<<UDRE)) {}
	UDR = znak; 
	return 0;
}

int rs_get(void)
{
	char znak; 
	// Oczekiwanie na pojawienie siê danej
	while(!(UCSRA & 1<<RXC)) {}
	znak = UDR; 
	return znak;
}
