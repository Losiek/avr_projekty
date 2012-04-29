#include <avr\io.h>
#include <inttypes.h>
//#include <avr\signal.h>
#include <avr\interrupt.h>

#include "harddef.h"

uint8_t count = 0;

// Start
int main(void) 
{
	////////////////////////////////////////////////////
	// Inicjacja
	LEDDDR = 0xff;
	COMDDR = 1<<COM1 | 1<<COM2 | 1<<COM3 | 1<<COM4;
	// Wygaszenie wy�wietlaczy
	LEDPORT = 0xff;
	COMPORT = 0xff;
	// Timer0
	TCCR0 = 1<<CS02 | 0<<CS01 | 1<<CS00;
	TIMSK = 1<<TOIE0;
	// Globalne zezwolenie przerwa�
	sei();
	// koniec inicjacji
	///////////////////////////////////////////////////
	
	for(;;)
	{
		if (count = 255) {
			count = 0;
			//W��czenie wy�wietlaczy
			COMPORT = ~(1<<COM1);
			//Zapalenie segmentu
			LEDPORT = ~(1<<LED_A);
			
			}
		}
		
	return 0;
}

//_____________________________________________________
// Obs�uga przerwa�
SIGNAL(TIMER0_OVF_vect)
{
	//Wpisanie do licznika pocz�tkowej warto�ci
	//TCNT0 = 128;
	++count;
	PORTB = ~(1<<LED_C);
}
