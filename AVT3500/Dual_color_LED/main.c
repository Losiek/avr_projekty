//Sterowanie PWM diodami LED wspóla katoda

#include <avr\io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "harddef.h"
#include "dzwiek.h"
#include "sounds.c"

int main(void) {
	////////////////////////////////////////
	// inicjacja
	
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
	
	//Muzyka
	///////////////////////////////////////
	play_G1(4);
	play_A1(4);
	play_E1(3);
	play_pause(3);
	play_G1(4);
	play_A1(4);
	play_D2(3);
	play_pause(3);
	play_G1(4);
	play_Fis(8);
	play_E1(8);
	play_D1(3);
	play_pause(3);
	play_G1(4);
	play_E1(1);
	play_E1(2);
	play_pause(2);
	play_pause(3);
	play_G1(4);
	play_A1(4);
	play_E1(3);
	play_pause(3);
	play_G1(4);
	play_A1(4);
	play_D2(3);
	play_pause(3);
	play_G1(4);
	play_Fis(8);
	play_E1(8);
	play_D1(3);
	play_pause(3);
	play_A1(4);
	play_E1(1);
	play_E1(2);
	play_pause(2);
	play_pause(3);
	play_pause(1);
	play_pause(1);
	play_pause(1);
	
	play_H0(8);
	play_E1(8);
	play_G1(8);
	play_E1(4);
	play_E2(1);
	play_E2(8);
	play_H0(8);
	play_E1(8);
	play_G1(8);
	play_E1(4);
	play_D2(1);
	play_D2(2);
	play_H0(8);
	play_E1(8);
	play_G1(8);
	play_E1(8);
	play_D2(8);
	play_H1(8);
	play_Fis(4);
	play_D1(8);
	play_A1(1);
	
	
	for(;;) {
		if(!(SWPIN & 1<<SW1)) {
			LEDPORT &= ~(1<<LED_blue);
			}
		LEDPORT = 1<<LED_blue;
		
	}
	
	return 0;
}
	

