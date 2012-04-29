#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <harddef.h>

int main()
{
	///////////////////////////////////////////////////////////////////
	//Inicacja wyœwietlaczy
	LEDDDR = 0xff;
	COMDDR = 1<<COM1|1<<COM2|1<<COM3|1<<COM4;
	//////////////////////////////////////////////////////////////////
	//Inicjacja PWM
	PORTB = (1<<4);
    TCCR1A = (1 << COM1A1); // zeruj gdy równe OCR1B
    TCCR1A |= (1 << PWM10); // PWM phase correct 8 bitowy
    TCCR1B = (1 << CS10); // preskaler 1 
	//Koniec inicjacji
	//////////////////////////////////////////////////////////////////
	PORTD = 0xff;
	//COMPORT = ~(1<<COM1);
	//LEDPORT = ~(1<<LED_A|1<<LED_B|1<<LED_C|1<<LED_D|1<<LED_E|1<<LED_F|1<<LED_G);
	OCR1AL = 0;
	OCR1AH = 0;
	
	for(;;)
	{
		_delay_ms(100);
		--OCR1AL;
	}
	return 0;
}
