#include "ip_protocol.h"

//Funkcja sprawdzaj¹ca datagram IP (ICMP czy TCP)
uint8_t IpCheck(void)
{
	if(EthFrame[IpProtocol]==0x01) return 3; 	//Je¿eli ICMP
	if(EthFrame[IpProtocol]==0x06) return 4; 	//Je¿eli TCP
	
	return 0;
}//koniec funkcji sprawdzaj¹cej 

//Funkcja obliczj¹ca sumê kontroln¹
uint16_t IpCheckSumCalc(void)
{
	EthFrame[IpCheckSum0]=0x00;
	EthFrame[IpCheckSum1]=0x00;
	
	uint16_t a;
	uint8_t i;
	unsigned long sum=0;
	
	for(i=0; i<20; i+=2)
	{
		a=((EthFrame[EthData+1]<<8)&0xFF00)+(EthFrame[EthData+i+1]&0xFF);
		sum=sum+(unsigned long)a;
	}
	
	while(sum>>16)
	{
		sum=(sum&0xFFFF)+(sum>>16);
	}
	
	sum=~sum;
	
	return ((unsigned int)sum);

}//Koniec funkcji obliczaj¹cej sumê kontroln¹ nag³ówka ramki IP

