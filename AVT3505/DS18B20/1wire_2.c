#include <avr/io.h>
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

/////////////////////////////////////////////////////////////////////////////////////////////
//Pozosta�e komendy przydatne do obs�ugi interfejsu 1Wire

void  SEARCH_ROM() { 						//Umo�liwia okre�lenie ilo�ci i rodzaj uk�ad�w Slave. Dane zapisywane do bufora 1Wire.
	pbufor1w=&bufor1w[0];					//Dane b�d� umieszczane w buforze "bufor1w".
	uint8_t i;								//Zmienna pomocnicza
	send_byte(0xf0); 						//wy�lij rozkaz SEARCH ROM
	for(i=0; i<8; i++) {
		*pbufor1w++=read_byte();			//osiem kolejnych bajt�w, zostanie wys�anych do bufora
	}
} //koniec SEARCH_ROM

void  READ_ROM() { 							//Umo�liwia odczyt kodu ROM z uk�adu podrz�dnego. Dane zapisywane do bufora 1Wire.
	pbufor1w=&bufor1w[0];					//Dane b�d� umieszczane w buforze "bufor1w".
	uint8_t i;								//Zmienna pomocnicza
	send_byte(0x33); 						//wy�lij rozkaz READ ROM
	for(i=0; i<8; i++) {
		*pbufor1w++=read_byte();			//osiem kolejnych bajt�w, zostanie wys�anych do bufora
	}
} //koniec READ_ROM

void  MATCH_ROM() { 						//Komenda ta wywo�ywana jest celem zaadresowania uk�adu podrz�dnego. Dane dane wysy�ane s� z bufora 1Wire.
	pbufor1w=&bufor1w[0];					//Dane b�d� umieszczane w buforze "bufor1w".
	uint8_t i;								//Zmienna pomocnicza
	send_byte(0x55); 						//wy�lij rozkaz MATCH ROM
	for(i=0; i<8; i++) {
		send_byte(*(uint8_t*)pbufor1w++);	//osiem kolejnych bajt�w, zostanie odczytanych z bufora i przes�anych do Slave.
	}
} //koniec MATCH_ROM

void  SKIP_ROM() { 							//Uk�ad nadrz�dny mo�e u�ywa� tej komendy po to aby zaadresowa� wszystkie urz�dzenia na magistrali r�wnocze�nie, z pomini�ciem wys�ania jakiegokolwiek kodu ROM.
	send_byte(0xCC); 						//wy�lij rozkaz SKIP ROM
} //koniec SKIP_ROM

