//Obs³uga uk³adu RTL8019as
//wyniki wysy³ane na UART

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "ex_ram.h"
#include "rs.h"
#include "RTL8019as.h"
#include "protocols/_all_protocols.h"


//Niezbêdna funkcja
void before_main(void)
	__attribute__((naked))
	__attribute__((section(".init3")));
	
void before_main(void)
{
	MCUCR = (1<<SRE /*| 1<<SRW10*/);
	EMCUCR = (1<<SRL2/*| 1<<SRW11 | 1<<SRW01 | 1<<SRW00*/); //Podzia³ na dwa sektory, dolny = 0x1100-0x7FFF
	//											 										górny = 0x8000-0xFFFF
	//RTL8019AS znajduje siê na                          								0x8300-0x831F
	//SFIOR = 1<<XMBK | 1<<XMM0;A15
}

//inicjacja zmiennej w exram o wielkoœci 1514 bajtów                  //umieszczanie w pamiêci od koñca
uint8_t EthFrame[1514]; //Ramka ethernet w pamiêci zewnêtrznej
uint8_t rtl_reg[32];
//Rozmiar pakietu
uint16_t PacketSize;
uint8_t MyMac[6] = { 0x00, 0x0A, 0x14, 0x1E, 0x28, 0x32 };

uint8_t ArpReplyIp[4];
uint8_t ServIp[4] = { 169, 254, 4, 58 };
uint8_t ArpReplyMac[6];

volatile int n = 0;
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
	////////////////////////////////////////////////////////
	//Inicjacja przerwañ
	MCUCR |= (1<<ISC01 | 1<<ISC00);
	GICR = 1<<INT0;
	sei();
	//Koniec inicjacji
	////////////////////////////////////////////////////////

	//inicjacja funkcji domyœlnych strumieni
	fdevopen(rs_put, rs_get);

	//wiadomoœæ powitalna
	printf("Witaj RTL8019as\n");
	printf("\r");								//zajmuje mniej pamiêci, serio
	
	RTLinit();
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

	printf("%p\n\r", &rtl_reg); 
	printf("%p\n\r", &EthFrame); 
	
	//content1[50] = "HTTP/1.0 200 OK\r\n Content-type: text/html\r\t\n";
	//printf("%s ",content1);
	/*for(uint16_t i=0; i<15; i++) {
		EthFrame[i]=1;
	}
	for(uint16_t i=0; i<1514; i++) {
		printf("%x ", EthFrame[i]);
	}*/
		
	for(;;) {
		/*while(RTLreceive_packet()!=0x01) {}
		//printf("receive %x ",(uint8_t)RTLreceive_packet());
		for(uint8_t i=0; i<15; i++) {
		printf("%x ", EthFrame[i]);
		}
		printf("\n\r");
		
		if(PacketCheck() == 1) {
			ArpCheck();
		} else if (PacketCheck() == 2) {
			if(IpCheck() == 3) { EchoReply(); }
			if(IpCheck() == 4) { Tcp_Check(0x1F90); }
		}
		Http_Check();*/
		printf("%d ", n);
	}
	
	return 0;
}

//Wyst¹pienie przerwania
ISR( INT0_vect )
{
  ++n;
}	