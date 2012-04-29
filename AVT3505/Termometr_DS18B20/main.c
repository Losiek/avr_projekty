//Obs³uga termometru 1wire ds18b20
//wyniki wysy³ane na UART

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "harddef.h"
#include "rs.h"

int main(void) {
	
	/////////////////////////////////////////////////////////
	//Inicjacja portu szeregowego
	RS_SET_BAUD(DEF_BAUD);
	UCSR0C = 1<<URSEL0 | 1<<UCSZ01 | 1<<UCSZ00;
	UCSR0B = 1<<RXEN0 | 1<<TXEN0;
	UCSR0A = 0;
	//Koniec inicjacji
	////////////////////////////////////////////////////////

	//inicjacja funkcji domyœlnych strumieni
	fdevopen(rs_put, rs_get);
	
	//Inicjacja zmiennych globalnych
	unsigned char T_LSB = 0x00;
	unsigned char T_MSB = 0x00;

//	unsigned char buffer[3] = "   ";
	
	//Wiadomoœæ powitalna
	printf("Termometr DS18B20\r\n", 0);

	for(;;) {
		
		reset_1w();
		send_1w(0xCC);
		send_1w(0x44);
		_delay_ms(500);
		reset_1w();
		send_1w(0xCC);
		send_1w(0xBE); 
		T_LSB = get_1w();
		T_MSB = get_1w();
		reset_1w();
		
		uint16_t Temp = T_MSB;
		Temp = Temp<<8;
		Temp = Temp |= T_LSB;
		float T1 = Temp/16.0;
		
		//Wysy³anie wyników
		printf("Temperatura = %d\r\n", T1);//zmiana
		

		}	
	return 0;
}
