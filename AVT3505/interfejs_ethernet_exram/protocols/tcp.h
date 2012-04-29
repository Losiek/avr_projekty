#ifndef TCP_PROTOCOL_H_INCLUDED
#define TCP_PROTOCOL_H_INCLUDED

#include <avr/io.h>
#include "RTL8019as.h"

#define EthData 			14

#define SourcePort0			EthData+20+0
#define SourcePort1			EthData+20+1

#define DestinPort0			EthData+20+2
#define DestinPort1			EthData+20+3

#define SeqNumber0			EthData+20+4
#define SeqNumber1			EthData+20+5
#define SeqNumber2			EthData+20+6
#define SeqNumber3			EthData+20+7

#define AckNumber0			EthData+20+8
#define AckNumber1			EthData+20+9
#define AckNumber2			EthData+20+10
#define AckNumber3			EthData+20+11

#define DataOffset			EthData+20+12 //wartoœæ 5

#define ControlBits			EthData+20+13

#define WindowSize			EthData+20+15

#define TcpCheckSum0		EthData+20+16
#define TcpCheckSum1		EthData+20+17

#define DataPointer0		EthData+20+18
#define DataPointer1		EthData+20+19

//flagi TCP
#define FIN 0x01
#define SYN 0x02
#define RST 0x04
#define PSH 0x08
#define ACK 0x10

//stany TCP
#define Closed		 	0x00
#define SynReceived 	0x01
#define SendSyn			0x02
#define Established 	0x03
#define FinWait1 		0x04
#define FinWait2 		0x05
#define Closing 		0x06
#define TimeWait 		0x07
#define CloseWait 		0x08
#define LastAck 		0x09

uint16_t dest_port;
uint32_t seq_number;
uint32_t ack_number;
uint16_t tcp_packet_count;
extern uint8_t Tcp_Status;
uint8_t Tcp_Status = Closed;
uint8_t next_send_data; //numer SEQ iloœci wys³anch bajtów danych
uint8_t next_ack;


uint16_t TcpCheckSumCalc(uint16_t dane); //iloœæ bajtów danych w pseudo-nag³ówku
void Tcp_Open_Active(uint16_t port);
void Tcp_Open_Pasive(uint16_t port);
void Tcp_Fin_Active(uint16_t port);
void Tcp_Fin_Pasive(uint16_t port);
void Tcp_Fin_Http(uint16_t port);
uint16_t Tcp_Send_Ack(uint16_t port);
void Tcp_Send_Data(uint16_t port, uint16_t data_lenght);
uint8_t Tcp_Receive(uint16_t port);
void Tcp_Check(uint16_t port);


#endif //TCP_PROTOCOL_H_INCLUDED