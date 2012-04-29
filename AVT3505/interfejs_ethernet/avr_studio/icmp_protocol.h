#ifndef ICMP_PROTOCOL_H_INCLUDED
#define ICMP_PROTOCOL_H_INCLUDED

#include <avr/io.h>
#include "RTL8019as.h"

#define EthData 			14

#define ImcpType			EthData+20+0
#define ImcpCode			EthData+20+1

#define Imcpsum0			EthData+20+2
#define	Imcpsum1			EthData+20+3

#define ImcpEchoId0			EthData+20+4
#define ImcpEchoId1			EthData+20+5

#define ImcpEchoNr0			EthData+20+6
#define ImcpEchoNr1			EthData+20+7

#define ImcpData			EthData+20+8

void EchoReply(void);

#endif //ICMP_PROTOCOL_H_INCLUDED
