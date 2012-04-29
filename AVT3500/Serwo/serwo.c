//Sterowanie serwomechnizmem
#include <util/delay.h>

void serwo(uint8_t angle, uint8_t speed) { //MAX angle = 100, 
	uint8_t a;
	while (a < 5) {
	/*if (angle > 100) {
		angle = 100;}*/
	SerwoPORT = 1<<Serwo;
	_delay_ms(/*1+*/(angle/100));
	SerwoPORT = ~(1<<Serwo);
	_delay_ms(20/*-(1+(angle/100))*/);
	a++;
	//_delay_ms(speed);
	}
}
	