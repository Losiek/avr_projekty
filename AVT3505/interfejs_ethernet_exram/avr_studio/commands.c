//obs³uga interfejsu

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "rs.h"
#include "RTL8019as.h"
#include "commands.h"
#include "arp_protocol.h"
#include "ip_protocol.h"

void command() {
char a;
	if (rs_get() == '-') {
		printf("-");
		switch (a=rs_get()) {
			case 'h':
				printf("%c\n\r",a);
				printf("Pomoc\n");
				break;
				
			case 'r':
				printf("%c\n\r",a);
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
					
					printf("\n\n\r");
				}
				break;
				
			case 's':
				printf("%c\n\r",a);
				printf("Rozmiar pakietu w bajtach - ");
				printf("%d B ", PacketSize);	
				printf("\n\n\r");
				break;
			
			case 'f':
				printf("%c\n\r",a);
				printf("Ramka ethernet - \n\r");
				for(uint16_t i = 0; i<PacketSize; i++) {
					if(i==0) {printf("MAC odbiorcy: ");}
					if(i==6) {printf("\n\rMAC nadawcy:   ");}
					if(i==12) {printf("\n\rtyp: ");}
					if(i==14) {printf("\r\n");}
					printf("%X ", EthFrame[i]);
				}
	
				printf("\n\n\r");
				break;
			
			case 'e':
				printf("%c\n\r",a);
				printf("Odbieranie pakietu - ");
				printf("%d",RTLreceive_packet());	
				printf("\n\n\r");
				break;
				
			case 'c':
				printf("%c\n\r",a);
				printf("Odbieranie wszystkich pakietów z bufora - ");
				while(!(RTLreceive_empty_check())) {
					RTLreceive_packet();
					RTLwrite(CR, 0x68);
					printf("curr = %x \n\r",  RTLread(CURR));
					RTLwrite(CR, 0x22);
					printf("bnry = %x \n\r",  RTLread(BNRY));
					printf("nextPacketPtr = %x\n\r",  nextPacketPtr);
				}
				printf("Zakoñczone");
				printf("\n\n\r");
				break;
				
			case 'a':
				printf("%c\n\r",a);
				printf("Odbieranie wiadomosci ARP - ");
				while(PacketCheck() != 1) {
					RTLreceive_packet();
				}
				printf("odebrano");
				ArpCheck();
				while(!(PacketCheck()==2 && IpCheck()==3)) {
					RTLreceive_packet();
				}
				//EchoReply();
				printf("wyslano echo");
				printf("\n\n\r");
				break;
				
			case 'i':
				printf("%c\n\r",a);
				printf("Adres IP mikroserwera: ");
				for(uint8_t i=0; i<4; i++) {
					printf("%d",ServIp[i]);
					if((i==0)|(i==1)|(i==2)) {printf(".");}
				}
				
				printf("\n\rAdres IP routera: ");
				for(uint8_t i=0; i<4; i++) {
					printf("%d",ArpReplyIp[i]);
					if((i==0)|(i==1)|(i==2)) {printf(".");}
				}
				printf("\n\n\r");
				break;
				
			case 'n':
				printf("%c\n\r",a);
				printf("Naglowek IP ");
				MakeIpHeader(1,0); 
				RTLsend_packet();	
				printf("\n\n\r");
				break;
				
			default:
				printf("%c\n\r",a);
				printf("Pomoc\n");
				break;
		}
	}
}
			