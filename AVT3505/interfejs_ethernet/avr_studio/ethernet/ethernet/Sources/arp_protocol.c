#include "arp_protocol.h"
#include "RTL8019as.h"

//Funkcja tworz¹ca zapytanie ARP
void MakeArpRequest(uint8_t destrIp1, uint8_t destrIp2, uint8_t destrIp3, uint8_t destrIp4)
{
	MakeEthHeader(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x08, 0x06);
	
	EthFrame[ArpMediumType0]=0x00;
	EthFrame[ArpMediumType1]=0x01;
	
	EthFrame[ArpProtocolType0]=0x08;
	EthFrame[ArpProtocolType1]=0x00;
	
	EthFrame[ArpHardLen]=0x06;
	
	EthFrame[ArpLogicLen]=0x04;
	
	EthFrame[ArpOpType0]=0x00;
	EthFrame[ArpOpType1]=0x01;
	
	EthFrame[ArpSourceMac0]=MyMac[0];
	EthFrame[ArpSourceMac1]=MyMac[1];
	EthFrame[ArpSourceMac2]=MyMac[2];
	EthFrame[ArpSourceMac3]=MyMac[3];
	EthFrame[ArpSourceMac4]=MyMac[4];
	EthFrame[ArpSourceMac5]=MyMac[5];
	
	EthFrame[ArpSourceIp1]=ServIp[0];
	EthFrame[ArpSourceIp2]=ServIp[1];
	EthFrame[ArpSourceIp3]=ServIp[2];
	EthFrame[ArpSourceIp4]=ServIp[3];
	
	EthFrame[ArpDestMac0]=0x00;
	EthFrame[ArpDestMac1]=0x00;
	EthFrame[ArpDestMac2]=0x00;
	EthFrame[ArpDestMac3]=0x00;
	EthFrame[ArpDestMac4]=0x00;
	EthFrame[ArpDestMac5]=0x00;
	
	EthFrame[ArpDestIp1]=destrIp1;
	EthFrame[ArpDestIp2]=destrIp2;
	EthFrame[ArpDestIp3]=destrIp3;
	EthFrame[ArpDestIp4]=destrIp4;
	
	PacketSize=PacketSize+28;

}//Koniec funkcji tworz¹cej zapytanie ARP

//Funkcja obs³uguj¹ca wiadomoœæ ARP
void ArpCheck(void)
{
	if(EthFrame[ArpMediumType0]==0x00 && EthFrame[ArpMediumType1]==0x01
		&& EthFrame[ArpProtocolType0]==0x08 && EthFrame[ArpProtocolType1] ==0x00
		&& EthFrame[ArpHardLen]==0x06 && EthFrame[ArpLogicLen]==0x04 
		&& EthFrame[ArpOpType0]==0x00 && EthFrame[ArpOpType1]==0x01
		&& EthFrame[ArpDestIp1]==ServIp[0] && EthFrame[ArpDestIp2]==ServIp[1]
		&& EthFrame[ArpDestIp3]==ServIp[2] && EthFrame[ArpDestIp4]==ServIp[3])
	{
		EthFrame[EthDestMac+0]=EthFrame[EthSourceMac+0];
		EthFrame[EthDestMac+1]=EthFrame[EthSourceMac+1];
		EthFrame[EthDestMac+2]=EthFrame[EthSourceMac+2];
		EthFrame[EthDestMac+3]=EthFrame[EthSourceMac+3];
		EthFrame[EthDestMac+4]=EthFrame[EthSourceMac+4];
		EthFrame[EthDestMac+5]=EthFrame[EthSourceMac+5];
		
		EthFrame[EthSourceMac+0]=MyMac[0];
		EthFrame[EthSourceMac+1]=MyMac[1];
		EthFrame[EthSourceMac+2]=MyMac[2];
		EthFrame[EthSourceMac+3]=MyMac[3];
		EthFrame[EthSourceMac+4]=MyMac[4];
		EthFrame[EthSourceMac+5]=MyMac[5];
		
		EthFrame[ArpOpType0]=0x00;
		EthFrame[ArpOpType1]=0x02;
		
		EthFrame[ArpSourceMac0]=MyMac[0];
		EthFrame[ArpSourceMac1]=MyMac[1];
		EthFrame[ArpSourceMac2]=MyMac[2];
		EthFrame[ArpSourceMac3]=MyMac[3];
		EthFrame[ArpSourceMac4]=MyMac[4];
		EthFrame[ArpSourceMac5]=MyMac[5];
		
		EthFrame[ArpDestIp1]=EthFrame[ArpSourceIp1];
		EthFrame[ArpDestIp2]=EthFrame[ArpSourceIp2];
		EthFrame[ArpDestIp3]=EthFrame[ArpSourceIp3];
		EthFrame[ArpDestIp4]=EthFrame[ArpSourceIp4];
		
		EthFrame[ArpSourceIp1]=ServIp[0];
		EthFrame[ArpSourceIp2]=ServIp[1];
		EthFrame[ArpSourceIp3]=ServIp[2];
		EthFrame[ArpSourceIp4]=ServIp[3];
		
		EthFrame[ArpDestMac0]=EthFrame[EthDestMac+0];
		EthFrame[ArpDestMac1]=EthFrame[EthDestMac+1];
		EthFrame[ArpDestMac2]=EthFrame[EthDestMac+2];
		EthFrame[ArpDestMac3]=EthFrame[EthDestMac+3];
		EthFrame[ArpDestMac4]=EthFrame[EthDestMac+4];
		EthFrame[ArpDestMac5]=EthFrame[EthDestMac+5];
		
		RTLsend_packet();
	}
	
	if(EthFrame[ArpMediumType0]==0x00 && EthFrame[ArpMediumType1]==0x01
		&& EthFrame[ArpProtocolType0]==0x08 && EthFrame[ArpProtocolType1] ==0x00
		&& EthFrame[ArpHardLen]==0x06 && EthFrame[ArpLogicLen]==0x04 
		&& EthFrame[ArpOpType0]==0x00 && EthFrame[ArpOpType1]==0x02
		&& EthFrame[ArpDestIp1]==ServIp[0] && EthFrame[ArpDestIp2]==ServIp[1]
		&& EthFrame[ArpDestIp3]==ServIp[2] && EthFrame[ArpDestIp4]==ServIp[3])
	{
		ArpReplyMac[0]=EthFrame[EthSourceMac+0];
		ArpReplyMac[1]=EthFrame[EthSourceMac+1];
		ArpReplyMac[2]=EthFrame[EthSourceMac+2];
		ArpReplyMac[3]=EthFrame[EthSourceMac+3];
		ArpReplyMac[4]=EthFrame[EthSourceMac+4];
		ArpReplyMac[5]=EthFrame[EthSourceMac+5];
		
		ArpReplyIp[0]=EthFrame[ArpSourceIp1];
		ArpReplyIp[1]=EthFrame[ArpSourceIp2];
		ArpReplyIp[2]=EthFrame[ArpSourceIp3];
		ArpReplyIp[3]=EthFrame[ArpSourceIp4];
		
		ArpReply=1;
		ArpCount=60;
		pingTimeout=100;
	}
}
