#include <avr/io.h>

#include "harddef.h"
#include "makra.h"

void boot(void) __attribute__((section(".bootloader")));

int main() 
{
	//Konfiguracja portów
	PORT(COMPORT) = 1<<COM1 | 1<<COM2 | 1<<COM3 | 1<<COM4;
	DDR(LEDPORT) =  0xff;
	DDR(COMPORT) =  1<<COM1 | 1<<COM3 | 1<<COM3 | 1<<COM4;
	
	//Wyswietlanie kreski
	PORT(LEDPORT) = ~(1<<LED_A);
	PORT(COMPORT) &= ~(1<<COM1);
	
	return 0;
}

void boot(void)
{
	//Konfiguracja portów
	PORT(COMPORT) = 1<<COM1 | 1<<COM2 | 1<<COM3 | 1<<COM4;
	DDR(LEDPORT) =  0xff;
	DDR(COMPORT) =  1<<COM1 | 1<<COM3 | 1<<COM3 | 1<<COM4;
	
	//Wyswietlanie kropki
	PORT(LEDPORT) = ~(1<<LED_DP);
	PORT(COMPORT) &= ~(1<<COM1);
	
	for(;;)
	{}
}
