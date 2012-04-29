#ifndef IP_PROTOCOL_H_INCLUDED
#define IP_PROTOCOL_H_INCLUDED

#include <avr/io.h>
#include "RTL8019as.h"

#define EthData 			14

#define IpVIHL				EthData+0 	//Razem wersja i IHL(domyœlnie 5) 0x45

#define IpTypeofService		EthData+1

#define IpTotalLength0		EthData+2
#define IpTotalLength1		EthData+3

#define IpIdenti0			EthData+4
#define IpIdenti1			EthData+5

#define IpFlaTotalOff0		EthData+6	//Razem flagi i przesuniêcie fragmentu
#define IpFlaTotalOff1		EthData+7	

#define IpTimeToLive		EthData+8

#define IpProtocol			EthData+9

#define	IpChecksum0			EthData+10
#define	IpChecksum1			EthData+11

#define IpSourceIp1			EthData+12
#define IpSourceIp2			EthData+13
#define IpSourceIp3			EthData+14
#define IpSourceIp4			EthData+15

#define IpDestIp1			EthData+16
#define IpDestIp2			EthData+17
#define IpDestIp3			EthData+18
#define IpDestIp4			EthData+19

uint8_t IpCheck(void);
void MakeIpHeader(uint8_t protocol, uint16_t data_lenght);
uint16_t IpCheckSumCalc(void);

#endif // IP_PROTOCOL_H_INCLUDED