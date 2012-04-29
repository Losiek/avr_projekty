#include "protocols/ip.h"
#include "protocols/arp.h"

#include "RTL8019as.h"

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
	EthFrame[IpChecksum0]=0x00;
	EthFrame[IpChecksum1]=0x00;
	
	unsigned int a;
	unsigned int i;
	unsigned long sum=0;
	
	for(i=0; i<20; i=i+2)
	{
		a=((EthFrame[EthData+i]<<8)&0xFF00)+(EthFrame[EthData+i+1]&0xFF);
		sum=sum+(unsigned long)a;
	}
	
	while(sum>>16)
	{
		sum=(sum&0xFFFF)+(sum>>16);
	}
	
	sum=~sum;
	
	return ((unsigned int)sum);

}//Koniec funkcji obliczaj¹cej sumê kontroln¹ nag³ówka ramki IP

//Funkcja tworz¹ca nag³ówek datagramu IP
void MakeIpHeader(uint8_t protocol, uint16_t data_lenght) //data_lenght d³ugoœæ danych, liczona w bajtach 
{
	MakeEthHeader(ArpReplyMac[0], ArpReplyMac[1], ArpReplyMac[2], ArpReplyMac[3], ArpReplyMac[4], ArpReplyMac[5], 0x08, 0x00);
	
	EthFrame[IpVIHL] = 0x45;
	EthFrame[IpTypeofService] = 0x00;
	EthFrame[IpTotalLength0] = (uint8_t)(8>>(data_lenght+20));
	EthFrame[IpTotalLength1] = (uint8_t)(data_lenght+20);
	EthFrame[IpIdenti0] = 0xaa;
	EthFrame[IpIdenti1] = 0x00;
	EthFrame[IpFlaTotalOff0] = 0x40;
	EthFrame[IpFlaTotalOff1] = 0x00;
	EthFrame[IpTimeToLive] = 0x40;
	EthFrame[IpProtocol] = protocol;
	
	EthFrame[IpSourceIp1] = ServIp[0];
	EthFrame[IpSourceIp2] = ServIp[1];
	EthFrame[IpSourceIp3] = ServIp[2];
	EthFrame[IpSourceIp4] = ServIp[3];
	
	EthFrame[IpDestIp1] = ArpReplyIp[0];
	EthFrame[IpDestIp2] = ArpReplyIp[1];
	EthFrame[IpDestIp3] = ArpReplyIp[2];
	EthFrame[IpDestIp4] = ArpReplyIp[3];
	
	uint16_t CheckSum = IpCheckSumCalc();
	EthFrame[IpChecksum0] = (uint8_t)((CheckSum)>>8);
	EthFrame[IpChecksum1] = (uint8_t)(CheckSum);
	
	//PacketSize += 20;

}//Koniec funkcju tworz¹cej nag³owek IP

			