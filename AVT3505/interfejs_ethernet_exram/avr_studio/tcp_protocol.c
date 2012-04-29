#include <util/delay.h>
#include <stdio.h>

#include "tcp_protocol.h"
#include "ip_protocol.h"
#include "arp_protocol.h"
#include "RTL8019as.h"


//Funkcja obliczj¹ca sumê kontroln¹
uint16_t TcpCheckSumCalc(uint16_t dane)
{
		
	EthFrame[TcpCheckSum0]=0x00;
	EthFrame[TcpCheckSum1]=0x00;
	
	//Sumowanie pseudo-nag³ówka Ip
	uint8_t Data[12];
	
	Data[0] = ServIp[0];
	Data[1] = ServIp[1];
	Data[2] = ServIp[2];
	Data[3] = ServIp[3];
	
	Data[4] = ArpReplyIp[0];
	Data[5] = ArpReplyIp[1];
	Data[6] = ArpReplyIp[2];
	Data[7] = ArpReplyIp[3];
	
	Data[8] = 0x00;
	Data[9] = 0x06;
	
	Data[10] = (uint8_t)((20+dane)>>8);
	Data[11] = (uint8_t)(20+dane);
	
	unsigned int a;
	unsigned int i;
	unsigned long sum=0;
	
	for(i=0; i<12; i=i+2)
	{
		a=((Data[i]<<8)&0xFF00)+(Data[i+1]&0xFF);
		sum=sum+(unsigned long)a;
	}
	
	for(i=0; i<20; i=i+2)
	{
		a=((EthFrame[EthData+20+i]<<8)&0xFF00)+(EthFrame[EthData+20+i+1]&0xFF);
		sum=sum+(unsigned long)a;
	}
	
	while(sum>>16)
	{
		sum=(sum&0xFFFF)+(sum>>16);
	}
	
	sum=~sum;
	return ((unsigned int)sum);
	
}//Koniec funkcji obliczaj¹cej sumê kontroln¹

//Oczekiwanie na pakiet TCP na konkretnym porcie (serwer w stanie LISTEN)
void Tcp_Listen(uint16_t port) {
	if(EthFrame[DestinPort0] == (uint8_t)(port>>8) && EthFrame[DestinPort1] == (uint8_t)(port) 
		&& EthFrame[ControlBits] == SYN) {
		//Odebrano flagê Syn
		Tcp_Status = SynReceived;
		
		dest_port = (EthFrame[SourcePort0]<<8);
		dest_port += EthFrame[SourcePort1];

		MakeIpHeader(0x06,0x14);
		
		EthFrame[DestinPort0] = EthFrame[SourcePort0];
		EthFrame[DestinPort1] = EthFrame[SourcePort1];
		
		EthFrame[SourcePort0] = (uint8_t)(port>>8);
		EthFrame[SourcePort1] = (uint8_t)(port);
		
		EthFrame[AckNumber0] = EthFrame[SeqNumber0];
		EthFrame[AckNumber1] = EthFrame[SeqNumber1];
		EthFrame[AckNumber2] = EthFrame[SeqNumber2];
		EthFrame[AckNumber3] = (EthFrame[SeqNumber3] + 1);
		
		EthFrame[SeqNumber0] = (uint8_t)(seq_number>>24); //Numer porz¹dkowy SEQ
		EthFrame[SeqNumber1] = (uint8_t)(seq_number>>16);
		EthFrame[SeqNumber2] = (uint8_t)(seq_number>>8);
		EthFrame[SeqNumber3] = (uint8_t)(seq_number);
		
		EthFrame[DataOffset] = 0x50;
		EthFrame[ControlBits] = (SYN|ACK);
		//EthFrame[14] = 0x00;
		EthFrame[WindowSize] = 0xff;
		
		uint16_t CheckSum = TcpCheckSumCalc(0x00);
		EthFrame[TcpCheckSum0] = (uint8_t)((CheckSum)>>8);
		EthFrame[TcpCheckSum1] = (uint8_t)(CheckSum);
		
		EthFrame[DataPointer0] = 0x00;
		EthFrame[DataPointer1] = 0x00;
		
		//Wysy³anie do RTL8019as
		PacketSize = 14+20+20;
		RTLsend_packet();
		
		//Czekanie na segment TCP z potwierdzeniem
		while((uint8_t)RTLreceive_packet()!=0x01) {}
		
		if(EthFrame[DestinPort0] == (uint8_t)(port>>8) && EthFrame[DestinPort1] == (uint8_t)(port) 
			&& EthFrame[ControlBits] == ACK && Tcp_Status == SynReceived
			&& EthFrame[AckNumber3] == next_send_data+0x01) {
			
			//Przepisz ack_number
			ack_number  = EthFrame[SeqNumber0]<<24;			//Numer potwierdzenia ACK
			ack_number += EthFrame[SeqNumber1]<<16;
			ack_number += EthFrame[SeqNumber2]<<8;	
			ack_number += EthFrame[SeqNumber3];
			
			printf(" %x ", EthFrame[SeqNumber0]);
			printf(" %x ", EthFrame[SeqNumber1]);
			printf(" %x ", EthFrame[SeqNumber2]);
			printf(" %x \n\r", EthFrame[SeqNumber3]);
			
			printf(" %x ", (uint8_t)(ack_number>>24));
			printf(" %x ", (uint8_t)(ack_number>>16));
			printf(" %x ", (uint8_t)(ack_number>>8));
			printf(" %x ", (uint8_t)(ack_number));
			
			//Ustanowiono po³¹czenie
			Tcp_Status = Established;
			printf("Tcp Status = %x", Tcp_Status);
			printf("\r\nTcp Status->Established");
		}
	}
}
	
//Zakoñczenie wirtualnego po³¹czenia
void Tcp_Fin(uint16_t port) {

	if(EthFrame[DestinPort0] == (uint8_t)(port>>8) && EthFrame[DestinPort1] == (uint8_t)(port) 
		&& EthFrame[ControlBits] == (FIN|ACK)) {
		Tcp_Status = CloseWait;

		MakeIpHeader(0x06,0x14);
		
		EthFrame[DestinPort0] = EthFrame[SourcePort0];
		EthFrame[DestinPort1] = EthFrame[SourcePort1];
		
		EthFrame[SourcePort0] = (uint8_t)(port>>8);
		EthFrame[SourcePort1] = (uint8_t)(port);
		
		EthFrame[AckNumber0] = EthFrame[SeqNumber0];
		EthFrame[AckNumber1] = EthFrame[SeqNumber1];
		EthFrame[AckNumber2] = EthFrame[SeqNumber2];
		EthFrame[AckNumber3] = (EthFrame[SeqNumber3] + 1);
		
		EthFrame[SeqNumber0] = 0x00; //Numer porz¹dkowy SEQ - 0x1001
		EthFrame[SeqNumber1] = 0x00;
		EthFrame[SeqNumber2] = 0x10;
		EthFrame[SeqNumber3] = 0x01;//(next_send_data+=1);
		
		EthFrame[DataOffset] = 0x50;
		EthFrame[ControlBits] = (FIN+ACK);
		EthFrame[WindowSize] = 0xff;
		
		uint16_t CheckSum = TcpCheckSumCalc(0x00);
		EthFrame[TcpCheckSum0] = (uint8_t)((CheckSum)>>8);
		EthFrame[TcpCheckSum1] = (uint8_t)(CheckSum);
		
		EthFrame[DataPointer0] = 0x00;
		EthFrame[DataPointer1] = 0x00;
		
		//Wysy³anie do RTL8019as
		PacketSize = 14+20+20;
		RTLsend_packet();
		
		//Czekanie na segment TCP z potwierdzeniem
		while(RTLreceive_packet()!=0x01) {}
		
		Tcp_Status = LastAck;
		next_send_data=0;
		next_ack=0;

		if(EthFrame[DestinPort0] == (uint8_t)(port>>8) && EthFrame[DestinPort1] == (uint8_t)(port) 
			&& EthFrame[ControlBits] == ACK && EthFrame[AckNumber3] == (EthFrame[SeqNumber3]+0x01)) {
			//Zakoñczono po³¹czenie
			Tcp_Status = ClosedListen;
			printf("\r\nTcp Status->Closed");
		}
	}
}

//Funkcja odczytuj¹ca dane TCP z konkretnego portu
uint8_t Tcp_Receive(uint16_t port) {

	if(EthFrame[DestinPort0] == (uint8_t)(port>>8) && EthFrame[DestinPort1] == (uint8_t)(port) 
		&& EthFrame[ControlBits] == (PSH+ACK) /*&& Tcp_Status == Established*/) {
		return 1; //Odebrano nowe dane
	}
	return 2; //B³¹d odbierania danych
}

//Funkcja wysy³aj¹ca potwierdzenie otrzymania danych TCP na konkretny port
uint16_t Tcp_Send_Ack(uint16_t port) {

		uint16_t receive_data = (EthFrame[IpTotalLength0]<<8); //iloœæ odebranych bajtów danych (20-nag³ówek IP, 20-nag³ówek TCP)
		receive_data += EthFrame[IpTotalLength1];
		receive_data -= (0x14+0x14);
		
		MakeIpHeader(0x06,0x14);
		
		EthFrame[DestinPort0] = EthFrame[SourcePort0];
		EthFrame[DestinPort1] = EthFrame[SourcePort1];
		
		EthFrame[SourcePort0] = (uint8_t)(port>>8);
		EthFrame[SourcePort1] = (uint8_t)(port);
		
		ack_number+=receive_data;
		
		EthFrame[AckNumber0] = (uint8_t)(ack_number>>24);
		EthFrame[AckNumber1] = (uint8_t)(ack_number>>16);
		EthFrame[AckNumber2] = (uint8_t)(ack_number>>8);
		EthFrame[AckNumber3] = (uint8_t)(ack_number);
		
		EthFrame[SeqNumber0] = (uint8_t)(seq_number>>24); //Numer porz¹dkowy SEQ
		EthFrame[SeqNumber1] = (uint8_t)(seq_number>>16);
		EthFrame[SeqNumber2] = (uint8_t)(seq_number>>8);
		EthFrame[SeqNumber3] = (uint8_t)(seq_number);
		
		EthFrame[DataOffset] = 0x50;
		EthFrame[ControlBits] = ACK;
		EthFrame[WindowSize] = 0xff;
		
		uint16_t CheckSum = TcpCheckSumCalc(0x00);
		EthFrame[TcpCheckSum0] = (uint8_t)((CheckSum)>>8);
		EthFrame[TcpCheckSum1] = (uint8_t)(CheckSum);
		
		EthFrame[DataPointer0] = 0x00;
		EthFrame[DataPointer1] = 0x00;
		
		//Wysy³anie do RTL8019as
		PacketSize = 14+20+20;
		RTLsend_packet();
		return receive_data;
}

//Funkcja wysy³aj¹ca dane TCP na konkretny port
void Tcp_Send_Data(uint16_t port, uint16_t data_lenght) {	
	
	MakeIpHeader(0x06,0x14+data_lenght);
	
	EthFrame[DestinPort0] = (uint8_t)(dest_port>>8);
	EthFrame[DestinPort1] = (uint8_t)(dest_port);
	
	EthFrame[SourcePort0] = (uint8_t)(port>>8);
	EthFrame[SourcePort1] = (uint8_t)(port);
	
	seq_number+=0x01;//data_lenght;
	EthFrame[SeqNumber0] = (uint8_t)(seq_number>>24); //Numer porz¹dkowy SEQ
	EthFrame[SeqNumber1] = (uint8_t)(seq_number>>16);
	EthFrame[SeqNumber2] = (uint8_t)(seq_number>>8);
	EthFrame[SeqNumber3] = (uint8_t)(seq_number/*+next_send_data*/);
	seq_number+=data_lenght;
	//next_send_data+=data_lenght;
	
	ack_number+=data_lenght+0x01;
	EthFrame[AckNumber0] = (uint8_t)(ack_number>>24);
	EthFrame[AckNumber1] = (uint8_t)(ack_number>>16);
	EthFrame[AckNumber2] = (uint8_t)(ack_number>>8);
	EthFrame[AckNumber3] = (uint8_t)(ack_number);
	//ack_number+=data_lenght;
	
	EthFrame[DataOffset] = 0x50;
	EthFrame[ControlBits] = (PSH|ACK);
	EthFrame[WindowSize] = 0xff;
		
	uint16_t CheckSum = TcpCheckSumCalc(data_lenght);
	EthFrame[TcpCheckSum0] = (uint8_t)((CheckSum)>>8);
	EthFrame[TcpCheckSum1] = (uint8_t)(CheckSum);
		
	EthFrame[DataPointer0] = 0x00;
	EthFrame[DataPointer1] = 0x00;
		
	//Wysy³anie do RTL8019as
	PacketSize = 14+20+20+data_lenght;
	RTLsend_packet();	
}

void Tcp_Check(uint16_t port) {
	if(PacketCheck() == 2 && IpCheck() == 4) {
		//Nas³uch na porcie
		Tcp_Listen(port);
		//Zamkniêcie po³¹czenia na porcie 
		Tcp_Fin(port);
	}

	/*if(Tcp_Receive(port) == 1) {
		Tcp_Send_Ack(port);
		
		EthFrame[14+20+20] = 'a';
		EthFrame[14+20+21] = 'b';
		EthFrame[14+20+22] = 'c';
		Tcp_Send_Data(port,0x03);
		EthFrame[14+20+20] = 0x0d;
		EthFrame[14+20+21] = 0x0a;
		Tcp_Send_Data(port,0x02);
	}*/
}
	