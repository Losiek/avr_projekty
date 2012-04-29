#include <inttypes.h>
#include <avr/io.h>
#include "1wire.h"

unsigned char bufor1w[9]; //bufor danych przychodz¹cych na magistralê 1-Wire.
unsigned char *pbufor1w; //wskaŸnik na bufor danych przychodz¹cych na magistralê 1-Wire.

//Pêtla opóznienia dla 1-Wire
//opóŸnienie jest równe t=(5+5*delay1w)*T, T-cykl zegarowy MCU, delay1w - parametr.
void wait_1w(uint8_t delay1w) {
	do {
		asm("nop"); //wstawka asemblerowa - rozkaz NOP
	}
	while(--delay1w!=0);
}

//Slot odczytu 1-Wire.
uint8_t slot1w_read(void) {
	
	//Zmienna
	uint8_t bit1w;
	
	wire_out0 								//inicjuj slot czytania
	wait_1w(1);								//Odczekaj
	wire_out1 								//zwolnij liniê
	wait_1w(14);							//ok. 14us
	bit1w = ((PIND&0x20)>>wire);
	wait_1w(59);							//ok. 60us
	return bit1w;
}

//Slot zapisu 1-Wire.
void slot1w_write(uint8_t znak) {
	
	if(znak) {								//slot "1"
		wire_out0							//inicjuj slot
		wait_1w(11);						//ok. 11us
		wire_out1
		
		wait_1w(60);
	} else {
		
		wire_out0                     //inicjuj slot
		wait_1w(70);						//ok. 70us
		wire_out1
	}
}

//Zapis bajtu do urz¹dzenia 1-Wire.
void write_1w(uint8_t rozkaz) {
	
	uint8_t i;								//Zmienna pomocnicza
	
	for(i=0;i<8;i++) {
		slot1w_write(rozkaz&0x01);			//Wyœlij bit
		rozkaz>>=1;
	}
}

//Odczyt bajtu z urz¹dzenia 1-Wire.
void read_1w(void) {
	
	uint8_t i,j,dana;
	
	pbufor1w=&bufor1w[0];					//Dane bêd¹ umieszczane w buforze "bufor1w".
	for(i=0;i<9;i++) {						//czytaj "family code" (1bajt)
											//i "registartion number" (7 bajtów)
		dana=0;								//wstêpne zerowanie danej
		
		for(j=0x01;j!=0;j<<=1) {			//zmienna steruj¹ca pêtli wskazuje jednoczeœnie aktualnie zapisywany bit
			dana|=slot1w_read()?j:dana;		//czytaj kolejne bity
		}
		*pbufor1w++=dana;					//po skompletowaniu zapisz do bufotra
	}
}

//Funkcja wyliczenia CRC.
void count_CRC(uint8_t bajt, uint8_t *CRC) {
											//wielomian generuj¹cy jest równy x^8 + x^5 + x^4 + 1
	uint8_t zp1, zp2,i; 					//Zmienne pomocniecze
	
	zp1=bajt;
	for(i=0;i<8;i++) {
		
		bajt^=*CRC;							//wskaŸnik *CRC wyznacza aktualnie wyliczony CRC
		
		zp2=bajt&0x01;						//wydzielenie bitu do obliczeñ
		bajt=*CRC;
		if(zp2) {
			bajt^=0x18;
			}
		bajt=((uint8_t)(bajt)>>1)+0x80*zp2;
		
		*CRC=bajt;
		zp1=(bajt=zp1>>1);
	}
}

//Sekwencja inicjuj¹ca 1-Wire.
uint8_t reset_1w(void) {
	wire_out0								//sekwencja inicjuj¹ca 1-Wire
	wait_1w(0xFF);
	wait_1w(0xB3);							//impuls inicjuj¹cy "0" ok. 480us
	wire_out1								//zwolnij liniê
	wait_1w(65);							//po odczekaniu ok. 65us czekaj na impuls obecnosci

	if(PIND&(1<<wire))  {
		return 1;							//zapamiêtaj fakt wykrycia urz¹dzenia
	}
	
	wait_1w(0xFF);
	wait_1w(0x4C);							//czekaj ok. 416us do zakonczenia slotu inicjuj¹cego
	
	if(!(PIND&(1<<wire))) {
		return 0;							//jeœli pozostaje w stanie niskim, to oznacza zwarcie
	}											//jeœli pozostaje w stanie niskim, to oznacza zwarcie
}
	
		

