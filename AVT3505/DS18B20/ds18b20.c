//Funkcje obs³ugi uk³adów DS18B20

#include "ds18b20.h"
#include "1wire_2.h"

void  CONVERT_T() { 						//Komenda ta inicjalizuje pojedyncz¹ konwersjê temperatury.
	send_byte(0x44); 						//wyœlij rozkaz CONVERT T
} //koniec CONVERT_T

void  WRITE_SCRATCHPAD() { 					//Komenda ta pozwala zapisaæ 3 bajty danych do uk³adu DS18B20.
	pbufor1w=&bufor1w[2];					//Dane bêd¹ pobierane z bufora.
											//pierwszy bajt - TH, drugi - TL, trzeci - bajt konfiguracyjny, DS18B20 Memory Map
	uint8_t i;								//Zmienna pomocnicza
	send_byte(0x4E); 						//wyœlij rozkaz WRITE SCRATCHPAD
	for(i=0; i<3; i++) {
		send_byte(*(uint8_t*)pbufor1w++);	//trzy kolejne bajty wysy³ane do SCRATCHPAD uk³adu.
	}
} //koniec WRITE SCRATCHPAD

void  READ_SCRATCHPAD() { 					//Komenda ta pozwala uk³adowi Master odczytaæ dane umieszczone w pamiêci  scratchpada.
	pbufor1w=&bufor1w[0];					//Dane bêd¹ umieszczane w buforze "bufor1w".
	uint8_t i;								//Zmienna pomocnicza
	send_byte(0xBE); 						//wyœlij rozkaz READ SCRATCHPAD
	for(i=0; i<8; i++) {
		*pbufor1w++=read_byte();			//osiem kolejnych bajtów, zostanie odczytanych z bufora i przes³anych do Slave.
	}
} //koniec READ SCRATCHPAD

void  COPY_SCRATCHPAD() { 					//Komenda ta pozwala na skopiowanie danych z rejestrów TH, TL oraz 
											//konfiguracyjnego (bajty 2, 3 i 4) do pamiêci EEPROM.
	send_byte(0x48); 						//wyœlij rozkaz COPY SCRATCHPAD
} //koniec COPY SCRATCHPAD

void  RECALL() { 							//Komenda ta wywo³uje alarmy wartoœci TH, TL oraz konfiguracyjne z pamiêci EEPROM i 
											//umiejscawia je na pozycjach 2, 3, 4 w pamiêci  scratchpada.
	send_byte(0xB8); 						//wyœlij rozkaz RECALL
} //koniec RECALL

void  READ_POWER_SUPPLY() { 				//Uk³ad Master mo¿e za¿¹daæ (poprzez wys³anie  read time slots) odpowiedzi, który uk³ad 
											//DS18B20 umieszczony na magistrali korzysta z zasilania paso¿ytniczego.
	send_byte(0xB4); 						//wyœlij rozkaz READ POWER SUPPLY
} //koniec READ POWER SUPPLY

float TEMP_READ() {    						//Pojedyñczy odczyt temperatury tylko jednego pod³¹czonego uk³adu!!!
	uint8_t temp1, temp2;					//Zmienne pomocnicze

	RESET_PULSE();
	send_byte(0xCC);
	send_byte(0x44);
	_delay_ms(750);							//czasu ulega zmianie w zale¿noœci od wybranej rozdzielczoœci
	
	RESET_PULSE();
	send_byte(0xCC);
	send_byte(0xBE);
	temp1=read_byte();
	temp2=read_byte();
	RESET_PULSE();
	
	return (float)(temp1+(temp2*256))/16;  //konwersja
} //koniec TEMP READ
	