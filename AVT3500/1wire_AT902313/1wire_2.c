#include <avr/io.h>
#include <util/delay.h>
#include "1wire_2.h"

//resetujemy magistrale - czekamy na impuls PRESENCE
uint8_t RESET_PULSE(void) {
	uint8_t PRESENCE;
	CLEAR_1Wire;
	_delay_us(500);
	SET_1Wire;
	_delay_us(30);
	//sprawdzamy poziom lini (czy w stanie niskim)
	if(bit_is_clear(PORT_1Wire, WE)) {
		PRESENCE=1;
	} else {
		PRESENCE=0;
	}
	//1-odebrano bit PRESENCE, 0-stan nieaktywno�ci
	_delay_us(470);
	if(bit_is_set(PORT_1Wire, WE)) {
		PRESENCE=1;
	} else {
		PRESENCE=0;
	}
	return PRESENCE; //zwracamy warto�� do funkcji
} //koniec RESET_PULSE

//wysy�a pojedy�czy bit do uk�adu
void send(uint8_t bit)
{
	CLEAR_1Wire; //ustawia stan niski na magistralii
	_delay_us(5);
	if(bit==1) {
		SET_1Wire; //zwolnienie magistralii - wyslanie jedynki
	}
	_delay_us(80); //przetrzymanie - wys�anie zera
	SET_1Wire;
} //koniec send

//zczytuje bit z magistaralii
uint8_t read(void) 
{
	uint8_t PRESENCE=0;
	
	CLEAR_1Wire; //ustawienie stan niski DQ
	_delay_us(2);
	SET_1Wire;
	_delay_us(15);
	//odbi�r zera lub jedynki
	if(bit_is_set(PORT_1Wire, WE)) {
		PRESENCE=1;
	} else {
		PRESENCE=0;
	}
	
	return(PRESENCE);
} //koniec read

//wysy�a ca�y bajt do uk�adu
void send_byte(uint8_t wartosc)
{
	uint8_t i; //zmienna licznikowa
	uint8_t pom; //zmienna pomocnicza
	
	for(i=0; i<8; i++) {
		pom = wartosc>>i;
		pom &= 0x01;
		send(pom);
	}
	_delay_us(100);
} //koniec send_byte

//zczytuje ca�y bajt z uk�adu
uint8_t read_byte(void)
{
	uint8_t i; //zmienna licznikowa
	uint8_t wartosc=0; //zczytywana wartosc
	
	for(i=0; i<8; i++) {
		if(read()) wartosc|=0x01<<i; //zczytywanie po jednym bicie
		_delay_us(15);
	}
	
	return(wartosc); //zwrot do funkcji
} //koniec read_byte
