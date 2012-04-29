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
	//Koniec inicjacji
	////////////////////////////////////////////////////////

	//inicjacja funkcji domyœlnych strumieni
	fdevopen(rs_put, rs_get);
	
	//inicjacja portów

	//wiadomoœæ powitalna
	printf("Witaj RTL8019as\n");
	printf("\r");								//zajmuje mniej pamiêci, serio
	
	RTLinit();
	seq_number=0x1000;

	for(;;) {

	while((uint8_t)RTLreceive_packet()!=0x01) {}
	
	if(PacketCheck() == 1) {
		ArpCheck();
	}
	
	Tcp_Check(0x50);
	
	Http_Check();
	}
	return 0;
}
	