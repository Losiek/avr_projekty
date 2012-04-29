//Obs³uga uk³adu RTL8019as
//wyniki wysy³ane na UART

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "rs.h"
//#include "RTL8019as.h"

//Ramka ethernet z pamiêci zewnêtrznej
uint8_t EthFrame[255]; //Ramka ethernet w pamiêci zewnêtrznej
//Rozmiar pakietu
uint16_t PacketSize= sizeof(EthFrame);
uint8_t MyMac[6] = { 0x00, 0x0A, 0x14, 0x1E, 0x28, 0x32 };

//funkcja wykonywana przed funkcj¹ main()
/*void before_main(void) __attribute__((naked)) __attribute__((section(".init3")));

void before_main(void) 
{
	MCUCR = 1<<SRE;
	SFIOR = 1<<XMBK | 1<<XMM0;
	DDRC = 0x80;
	PORTC &= ~(1<<7);
}*/

int main(void) 
{
	
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
	
	//inicjacja portów

	//wiadomoœæ powitalna
	printf("Witaj RTL8019as\n");
	printf("\r");								//zajmuje mniej pamiêci, serio
	
	RTLinit();
	
	//Odczytywanie rejestrów RTL8019as
	printf("\nRealtek 8019AS Register dump\n");
	printf("\r");
	printf("\nREG Page0 Page1 Page2 Page3\n\n");
	printf("\r");
		
	for(uint8_t reg = 0; reg<16; reg++) {
		
		uint8_t a = 0;
		printf("%X    ", reg);
		
		RTLwrite(0x00, 0x00);
		_delay_ms(2);
		a = RTLread(reg);
		if(a < 16) {
			printf("0");
			printf("%X    ", a);
			} else {
			printf("%X    ", a);
			}
		
		RTLwrite(0x00, 0x40);
		_delay_ms(2);
		a = RTLread(reg);
		if(a < 16) {
			printf("0");
			printf("%X    ", a);
			} else {
			printf("%X    ", a);
			}
		
		RTLwrite(0x00, 0x80);
		_delay_ms(2);
		a = RTLread(reg);
		if(a < 16) {
			printf("0");
			printf("%X    ", a);
			} else {
			printf("%X    ", a);
			}
		
		RTLwrite(0x00, 0xC0);
		_delay_ms(2);
		a = RTLread(reg);
		if(a < 16) {
			printf("0");
			printf("%X    ", a);
			} else {
			printf("%X    ", a);
			}
		
		printf("\n\r");
		}
	uint8_t cu, bn;
	
	
	printf("%d ", PacketSize);	
	
	uint8_t i=1;
	uint8_t y;
	
	MakeArpRequest(0x00, 0x00, 0x00, 0x00);
	RTLsend_packet();
	
	for(;;) {
	/*RTLwrite(0x00, 0x68);
	printf("CURR %X ",RTLread(0x07));
	printf("BNRY %X ",RTLread(0x03));*/
	while(i) {
	if(!(RTLreceive_empty_check())) {
		
		RTLreceive_packet();
		uint8_t pa=PacketCheck();
		
		if(pa==2) {
			printf("odebrano pakiet IP\n\r");
		}
		if(pa==1) {
			printf("odebrano wiadomosc ARP\n\r");
			ArpCheck();
		}
		if(pa==0) {
			printf("nieznany protokol\n\r");
		}
		
		//printf("odebrano dane\r\n");
		//printf("%d ", RTLreceive_packet());
		//RTLreceive_packet();
		//RTLreceive_packet();
		
		//printf("%d ", (uint8_t)RTLreceive_packet());
		//RTLreceive_packet();
		
		RTLwrite(0x00, 0x68);
		cu = RTLread(0x07);
		printf("CURR= %X ",cu);
		RTLwrite(0x00, 0x22);
		bn = RTLread(0x03);
		printf("BNRY= %X ",bn);
		/*
		y = RTLreceive_packet();
		printf("%d ",y);
		
		RTLwrite(0x00, 0x68);
		cu = RTLread(0x07);
		printf("CURR= %X ",cu);
		RTLwrite(0x00, 0x22);
		bn = RTLread(0x03);
		printf("BNRY= %X ",bn);
		*/
		/*printf("\n\r");
		for(uint16_t a=0; a<100; a++) {
			
			printf("%X-",EthFrame[a]);*/
		//RTLend_retreive();
		for(uint16_t a=0; a<(uint16_t)PacketSize; a++) {
			printf("%X-",EthFrame[a]);
		}
		
		--i;
		
		
	}
	}
	
		
	/*for(uint8_t i = 0; i<PacketSize; PacketSize++) {
	printf("%X ", EthFrame[i]);
	}*/
		
	/*for(uint8_t i = 0; i<10; i++) {
	printf("%X ", EthFrame[i]);
	}*/
	
	//RTLsend_packet();
	
	}
	
	return 0;
}
	