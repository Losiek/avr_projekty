#ifndef ARP_PROTOCOL_H_INCLUDED
#define ARP_PROTOCOL_H_INCLUDED

#include <avr/io.h>
#include "RTL8019as.h"

#define EthData 			14

#define ArpMediumType0		EthData+0
#define ArpMediumType1		EthData+1

#define ArpProtocolType0	EthData+2
#define ArpProtocolType1 	EthData+3

#define ArpHardLen			EthData+4

#define ArpLogicLen			EthData+5

#define ArpOpType0			EthData+6
#define ArpOpType1			EthData+7

#define ArpSourceMac0		EthData+8
#define ArpSourceMac1		EthData+9
#define ArpSourceMac2		EthData+10
#define ArpSourceMac3		EthData+11
#define ArpSourceMac4		EthData+12
#define ArpSourceMac5		EthData+13

#define ArpSourceIp1		EthData+14
#define ArpSourceIp2		EthData+15
#define ArpSourceIp3		EthData+16
#define ArpSourceIp4		EthData+17

#define ArpDestMac0 		EthData+18
#define ArpDestMac1 		EthData+19
#define ArpDestMac2 		EthData+20
#define ArpDestMac3 		EthData+21
#define ArpDestMac4 		EthData+22
#define ArpDestMac5 		EthData+23

#define ArpDestIp1			EthData+24
#define ArpDestIp2			EthData+25
#define ArpDestIp3			EthData+26
#define ArpDestIp4			EthData+27

//Deklaracje zmiennych
extern uint8_t ArpReplyMac[];
extern uint8_t ArpReplyIp[];
uint8_t ArpReply, ArpCount, pingTimeout;

//Aders IP mikroserwera
extern uint8_t ServIp[];

void MakeEthHeader(uint8_t dest0, uint8_t dest1, uint8_t dest2, uint8_t dest3, uint8_t dest4, uint8_t dest5, uint8_t type0, uint8_t type1);
void MakeArpRequest(uint8_t destrIp1, uint8_t destrIp2, uint8_t destrIp3, uint8_t destrIp4);
uint8_t ArpCheck(void);

#endif // ARP_PROTOCOL_H_INCLUDED