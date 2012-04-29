//Obs³uga uk³adu RTL8019as
//wyniki wysy³ane na UART

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "rs.h"
#include "RTL8019as.h"
#include "arp_protocol.h"
#include "ip_protocol.h"
#include "tcp_protocol.h"
#include "http_protocol.h"

//Ramka ethernet z pamiêci zewnêtrznej
uint8_t EthFrame[350]; //Ramka ethernet w pamiêci zewnêtrznej
//Rozmiar pakietu
uint16_t PacketSize;
uint8_t MyMac[6] = { 0x00, 0x0A, 0x14, 0x1E, 0x28, 0x32 };

uint8_t ArpReplyIp[4];
uint8_t ServIp[4] = { 169, 254, 4, 58 };
uint8_t ArpReplyMac[6];

//Status TCP
uint8_t Tcp_Status;

int main(void) 
{
	
	/////////////////////////////////////////////////////////
	//Inicjacja portu szeregowego
	RS_SET_BAUD(DEF_BAUD);
	UCSR0C = 1<<URSEL0 | 1<<UCSZ01 | 1<<UCSZ00;
	UCSR0B = 1<<RXEN0 | 1<<TXEN0;
	UCSR0A = 0;
	DDRE = 1<<PORTE1;
	PORTE = 1<<PORTE1;
	//Koniec inicjacji
	////////////////////////////////////////////////////////

	//inicjacja funkcji domyœlnych strumieni
	fdevopen(rs_put, rs_get);
	
	//inicjacja portów

	//wiadomoœæ powitalna
	printf("Witaj RTL8019as\n");
	printf("\r");								//zajmuje mniej pamiêci, serio
	
	RTLinit();
	
	RTLwrite(0x00, 0x00);	
	RTL_DDR = 0xff;
	PORTE = 1<<PORTE1;
	RTL_PORT = 0x07;
	PORTE = ~(1<<PORTE1);
	RTL_PORT = 0x33;
	
	RTL_CLEAR_WRITE;
	RTL_SET_WRITE;
	
	seq_number=0x1000;
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


	for(;;) {
	
	/*
	while((uint8_t)RTLreceive_packet()!=0x01) {}
	
	if(PacketCheck() == 1) {
		ArpCheck();
	} else if (PacketCheck() == 2) {
		if(IpCheck() == 3) {
			EchoReply();
		} else if (IpCheck() == 4) {
			Tcp_Check(0x1F90);
		}	
	}
	Http_Check();
	*/
	return 0;
}
}
	