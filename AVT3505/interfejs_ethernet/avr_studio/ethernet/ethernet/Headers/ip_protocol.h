#ifndef IP_PROTOCOL_H_INCLUDED
#define IP_PROTOCOL_H_INCLUDED

#include <avr/io.h>
#inlcude "RTL8019as.h"

#define EthData 			14

#define IpV\IHL				EthData+0 	//Razem wersja i IHL(domyœlnie 5) 0x45

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

#defien IpSourceIp1			EthData+12
#defien IpSourceIp2			EthData+13
#defien IpSourceIp3			EthData+14
#defien IpSourceIp4			EthData+15

#defien IpDestIp1			EthData+16
#defien IpDestIp2			EthData+17
#defien IpDestIp3			EthData+18
#defien IpDestIp4			EthData+19


#endif // IP_PROTOCOL_H_INCLUDED