#include "icmp_protocol.h"
#include "ip_protocol.h"
#include "RTL8019as.h"

//Funkcja odpowiadaj¹ca na ¿¹danie echa 
void EchoReply(void)
{
	if(EthFrame[ImcpType]==0x08 && EthFrame[ImcpCode]==0x00) {
		
		MakeIpHeader(0x01,0x12);
		
		uint16_t CheckSum = (8<<EthFrame[Imcpsum0]);
		CheckSum+=EthFrame[Imcpsum1];
		
		EthFrame[ImcpCode]=0x01;
		
		if(CheckSum > 0x7FFF) {
			CheckSum = (uint16_t)((CheckSum+0x8000)|0x01);
		} else {
			CheckSum = CheckSum+0x8000;
		}
		
		EthFrame[Imcpsum0]=(uint8_t)((CheckSum)>>8);
		EthFrame[Imcpsum1]=(uint8_t)(CheckSum);
		
		PacketSize = 14+20+12;
		RTLsend_packet();
	}
}

		