#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "rs.h"
#include "harddef.h"
#include "lis35.h"

int main(void)
{
	signed char XPos, YPos, ZPos;
	signed char iXPos, iYPos, iZPos;
	
	///////////////////////////////////////////////////
	//Inicjacja portu szeregowego
	RS_SET_BAUD(DEF_BAUD);
	UCSR0C = 1<<URSEL0 | 1<<UCSZ01 | 1<<UCSZ00;
	UCSR0B = 1<<RXEN0 | 1<<TXEN0;
	UCSR0A = 0;
	//Inicjacja akcelerometru LIS35DE
	InitializeLIS35();
	//Koniec inicjacji
	//////////////////////////////////////////////////
	
	//inicjacja funkcji domyœlnych strumieni
	fdevopen(rs_put, rs_get);
	
	//Wiadomoœæ powitalna
	puts( "Akcelerometr LIS35\r\n");

	if (InitializeLIS35()!=LIS35_OK)
	{
		while(1)
		{
			// When initialization fails send message
			printf("LIS35 inicjalisation is fail\r\n");
			_delay_ms(250); 
		}
	}

	while(1)
	{
		LIS35_GetPosition(&XPos, &YPos, &ZPos);

		//Send angle values
		printf( "X=%d\r\n"
				"Y=%d\r\n"
				"Z=%d\r\n",
				iXPos, iYPos, iZPos);
		_delay_ms(500);
	}
}


