#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "rs.h"
#include "harddef.h"
#include "i2c.h"

int main(void)
{

	///////////////////////////////////////////////////
	//Inicjacja portu szeregowego
	RS_SET_BAUD(DEF_BAUD);
	UCSR0C = 1<<URSEL0 | 1<<UCSZ01 | 1<<UCSZ00;
	UCSR0B = 1<<RXEN0 | 1<<TXEN0;
	UCSR0A = 0;
	//Koniec inicjacji
	//////////////////////////////////////////////////
	
	//inicjacja funkcji domyœlnych strumieni
	fdevopen(rs_put, rs_get);
	
	//Wiadomoœæ powitalna
	puts( "Kontrola I2C\r\n");

	//Konfiguracja przetwornika
	i2c_start();
	//Bajt adresowy, zapis
	i2c_send(0x90);
	//bajt kontrolny
	i2c_send(0x00);
	i2c_stop();
	
	//Pobieranie danych
	for(;;)
	{
		//Odczyt danych
		i2c_start();
		//Bajt adresowy, odczyt
		i2c_send(0x91);
		//Pobieranie i wyœwietlenie danej
		printf("%d\n\r", i2c_get(I2C_NACK));
		i2c_stop();
	}
}


