//Funkcje obs�ugi uk�ad�w DS18B20

#include "ds18b20.h"
#include "1wire_2.h"

void  CONVERT_T() { 						//Komenda ta inicjalizuje pojedyncz� konwersj� temperatury.
	send_byte(0x44); 						//wy�lij rozkaz CONVERT T
} //koniec CONVERT_T

void  WRITE_SCRATCHPAD() { 					//Komenda ta pozwala zapisa� 3 bajty danych do uk�adu DS18B20.
	pbufor1w=&bufor1w[2];					//Dane b�d� pobierane z bufora.
											//pierwszy bajt - TH, drugi - TL, trzeci - bajt konfiguracyjny, DS18B20 Memory Map
	uint8_t i;								//Zmienna pomocnicza
	send_byte(0x4E); 						//wy�lij rozkaz WRITE SCRATCHPAD
	for(i=0; i<3; i++) {
		send_byte(*(uint8_t*)pbufor1w++);	//trzy kolejne bajty wysy�ane do SCRATCHPAD uk�adu.
	}
} //koniec WRITE SCRATCHPAD

void  READ_SCRATCHPAD() { 					//Komenda ta pozwala uk�adowi Master odczyta� dane umieszczone w pami�ci  scratchpada.
	pbufor1w=&bufor1w[0];					//Dane b�d� umieszczane w buforze "bufor1w".
	uint8_t i;								//Zmienna pomocnicza
	send_byte(0xBE); 						//wy�lij rozkaz READ SCRATCHPAD
	for(i=0; i<8; i++) {
		*pbufor1w++=read_byte();			//osiem kolejnych bajt�w, zostanie odczytanych z bufora i przes�anych do Slave.
	}
} //koniec READ SCRATCHPAD

void  COPY_SCRATCHPAD() { 					//Komenda ta pozwala na skopiowanie danych z rejestr�w TH, TL oraz 
											//konfiguracyjnego (bajty 2, 3 i 4) do pami�ci EEPROM.
	send_byte(0x48); 						//wy�lij rozkaz COPY SCRATCHPAD
} //koniec COPY SCRATCHPAD

void  RECALL() { 							//Komenda ta wywo�uje alarmy warto�ci TH, TL oraz konfiguracyjne z pami�ci EEPROM i 
											//umiejscawia je na pozycjach 2, 3, 4 w pami�ci  scratchpada.
	send_byte(0xB8); 						//wy�lij rozkaz RECALL
} //koniec RECALL

void  READ_POWER_SUPPLY() { 				//Uk�ad Master mo�e za��da� (poprzez wys�anie  read time slots) odpowiedzi, kt�ry uk�ad 
											//DS18B20 umieszczony na magistrali korzysta z zasilania paso�ytniczego.
	send_byte(0xB4); 						//wy�lij rozkaz READ POWER SUPPLY
} //koniec READ POWER SUPPLY

float TEMP_READ() {    						//Pojedy�czy odczyt temperatury tylko jednego pod��czonego uk�adu!!!
	uint8_t temp1, temp2;					//Zmienne pomocnicze

	RESET_PULSE();
	send_byte(0xCC);
	send_byte(0x44);
	_delay_ms(750);							//czasu ulega zmianie w zale�no�ci od wybranej rozdzielczo�ci
	
	RESET_PULSE();
	send_byte(0xCC);
	send_byte(0xBE);
	temp1=read_byte();
	temp2=read_byte();
	RESET_PULSE();
	
	return (float)(temp1+(temp2*256))/16;  //konwersja
} //koniec TEMP READ
	