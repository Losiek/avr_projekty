#ifndef RTL8019as_H_INCLUDED
#define RTL8019as_H_INCLUDED

#include "ex_ram.h"

//definicja u³atwiaj¹ca korzystanie z kontrolera
#define RTL __attribute__ ((section (".rtl")))

//Deklaracje sygna³ów steruj¹cych kontrolerem RTL8019AS

#define RTL_PORT 					PORTA
#define RTL_DDR 					DDRA
#define RTL_PIN 					PINA

#define RTL_CONTROL_PORT 			PORTD
#define RTL_CONTROL_DDR 			DDRD

#define RTL_CONTROL_READPIN 		7
#define RTL_CONTROL_WRITEPIN 		6
#define RTL_CONTROL_INT0		 	2
#define RTL_CONTROL_AEN				4

#define RTL_CLEAR_READ 				RTL_CONTROL_PORT &= ~(1<<RTL_CONTROL_READPIN);
#define RTL_SET_READ 				RTL_CONTROL_PORT |= (1<<RTL_CONTROL_READPIN);
#define RTL_CLEAR_WRITE 			RTL_CONTROL_PORT &= ~(1<<RTL_CONTROL_WRITEPIN);
#define RTL_SET_WRITE 				RTL_CONTROL_PORT |= (1<<RTL_CONTROL_WRITEPIN);

#define RTL_RESET_PORT 				PORTD
#define RTL_RESET_DDR 				DDRD
#define RTL_RESET_PIN 				5

#define RTL_ON 		RTL_CONTROL_PORT &=~(1<<RTL_CONTROL_AEN);
#define RTL_OFF 	RTL_CONTROL_PORT |=(1<<RTL_CONTROL_AEN);

//Deklaracja wartoœci wpisywanych do rejestrów
#define CR 							0x00 
#define PSTART 						0x01
#define PAR0 						0x01
#define CR9346 						0x01
#define PSTOP 						0x02
#define BNRY 						0x03
#define TSR 						0x04
#define TPSR 						0x04
#define TBCR0 						0x05
#define NCR 						0x05
#define TBCR1 						0x06
#define _ISR 						0x07
#define CURR 						0x07
#define RSAR0 						0x08
#define CRDA0 						0x08
#define RSAR1 						0x09
#define CRDA1 						0x09
#define RBCR0 						0x0A
#define RBCR1 						0x0B
#define RSR 						0x0C
#define RCR 						0x0C
#define TCR 						0x0D
#define CNTR0 						0x0D
#define DCR 						0x0E
#define CNTR1 						0x0E
#define IMR 						0x0F
#define CNTR2 						0x0F
#define RDMAPORT 					0x10
#define RSTPORT 					0x18

#define ISR_PRX 					0
#define ISR_PTX 					1
#define ISR_OVW 					4
#define ISR_RDC 					6
#define ISR_RST 					7

#define TXSTART_INIT 				0x40
#define RXSTART_INIT 				0x46
#define RXSTOP_INIT 				0x60

#define ETHERNET_MIN_PACKET_LENGTH 	0x3C //60
#define ETHERNET_HEADER_LENGTH 		0x0E //14

#define IP_TCP_HEADER_LENGTH 		40
#define TOTAL_HEADER_LENGTH 		(IP_TCP_HEADER_LENGTH+ETHERNET_HEADER_LENGTH)

#define EthDestMac 					0x00
#define EthSourceMac				0x06
#define EthTypeLen					0x0C

//Deklaracje zmiennych
extern uint8_t MyMac[/*6*/]/* = { 0x00, 0x0A, 0x14, 0x1E, 0x28, 0x32 }*/;
extern uint8_t EthFrame[1514] EXMEM; //Ramka ethernet w pamiêci zewnêtrznej
extern uint8_t rtl_reg[32] RTL;
extern uint16_t PacketSize/* = sizeof(EthFrame)*/; //Rozmiar pakietu danych
extern unsigned char *rtlr;

uint8_t dma;
uint16_t nextPacketPtr;
uint16_t currentPacketPtr;
uint16_t currentRetreiveAddress;

void RTLsetup_ports(void);
void RTLhw_reset(void);
void RTLwrite (uint8_t address, uint8_t data); 
uint8_t RTLread(uint8_t address); 
void RTLinit(void);
void RTLsend_packet(void);
uint8_t RTLreceive_empty_check(void); 
void RTLend_retreive(void);
uint8_t RTLreceive_packet(void);
uint8_t PacketCheck(void);

#endif // RTL8019as_H_INCLUDED
