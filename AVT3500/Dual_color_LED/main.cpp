//Sterowanie PWM diodami LED wspóla katoda

#include <avr\io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "harddef.h"
#include "dzwiek.h"

//Funkcje
////////////////////////////////////////////
void play_C1() {
	float frec = (1/(2*C1*0x3e8));
	int frequency = static_cast<int>(frec);
	for (;;) {
		PORTPiez = 1<<Piezo;
		_delay_ms(frequency);
		PORTPiez = ~(1<<Piezo);
		_delay_ms(frequency);
		}
	}

int main(void) {
	////////////////////////////////////////
	// inicjacja
	LEDDDR = 1<<LED_blue;
	LEDPORT = 1<<LED_blue;
	
	SWDDR = ~(1<<SW1 | 1<<SW2);
	SWPORT = (1<<SW1 | 1<<SW2);
	
	PORTPiez = 1<<Piezo;
	PiezDDR = 1<<Piezo;
	//TCCR0A = 1<<COM0A1 | 0<<COM0A0 | 1<<COM0B1 | 0<<COM0B0 | 1<<WGM01 | 1<<WGM00;
	//TCCR0B = 1<<WGM02 | 0<<CS02 | 0<<CS01 | 1<<CS00; 
	//TCNT1 = 0xFF; // wartoœæ pocz¹tkowa T/C1

	// koniec inicjacji
	///////////////////////////////////////
	//OCR0A = 0x00;
	for(;;) {
		if(!(SWPIN & 1<<SW1)) {
			LEDPORT &= ~(1<<LED_blue);
			}
		LEDPORT = 1<<LED_blue;
		play_C1();
	}
	
	return 0;
}
	

