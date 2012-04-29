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
    TCCR1A = (1 << WGM10); 
    TCCR1A = (1 << COM1A0) | (1 << COM1A1); // PWM phase correct 8 bitowy
    TCCR1B = (1 << CS10); // preskaler 8 
	//
	TCCR2 = (1 << WGM20); 
    TCCR2 = (1 << COM20) | (1 << COM21); // PWM phase correct 8 bitowy
    TCCR2 = (1 << CS20); // preskaler 8 
	//TIMSK = 0x00;
	//Koniec inicjacji
	//////////////////////////////////////////////////////////////////
	PORTD = 0xff;
	//COMPORT = ~(1<<COM1);
	//LEDPORT = ~(1<<LED_A|1<<LED_B|1<<LED_C|1<<LED_D|1<<LED_E|1<<LED_F|1<<LED_G);
	PWM_1 = 0xff;
	PWM_2 = 0x00;
	OCR1B = 0xff;
	
	for(;;)
	{
		_delay_ms(50);
		--PWM_1;
	}
	return 0;
}
