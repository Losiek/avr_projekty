#include <util/delay.h>
#include <stdio.h>

#include "RTL8019as.h"
#include "protocols/tcp.h"
#include "protocols/ip.h"
#include "protocols/arp.h"


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
void Tcp_Open_Pasive(uint16_t port) {
	if(EthFrame[DestinPort0] == (uint8_t)(port>>8) && EthFrame[DestinPort1] == (uint8_t)(port) 
		&& EthFrame[ControlBits] == SYN) {
		//Odebrano flagê Syn
		Tcp_Status = SynReceived;
		
		dest_port = (EthFrame[SourcePort0]<<8);
		dest_port += EthFrame[SourcePort1];

		MakeIpHeader(0x06,0x14);
		
		EthFrame[SourcePort0] = (uint8_t)(port>>8);
		EthFrame[SourcePort1] = (uint8_t)(port);
		
		EthFrame[DestinPort0] = (uint8_t)(dest_port>>8);
		EthFrame[DestinPort1] = (uint8_t)(dest_port);
		
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
		uint8_t i=0x00;
		
		for(i=0x00; i<10; i++) { //Je¿eli nie bêdzie ACK w 10 nastêpnych pakietach po³¹czenie nie zostanie ustanowione
			while((uint8_t)RTLreceive_packet()!=0x01) {}
			
			if(EthFrame[DestinPort0] == (uint8_t)(port>>8) && EthFrame[DestinPort1] == (uint8_t)(port) 
				&& EthFrame[ControlBits] == ACK && Tcp_Status == SynReceived
				&& EthFrame[AckNumber3] == next_send_data+0x01) {
				
				//Przepisz ack_number
				ack_number  = (uint32_t)EthFrame[SeqNumber0]<<24;			//Numer potwierdzenia ACK
				ack_number += (uint32_t)EthFrame[SeqNumber1]<<16;
				ack_number += (uint32_t)EthFrame[SeqNumber2]<<8;	
				ack_number += (uint32_t)EthFrame[SeqNumber3];
			
				//Ustanowiono po³¹czenie
				Tcp_Status = Established;
				seq_number+=0x01;
				break;
			}
		}
		
	}
}

//Aktywne otwarcie po³¹czenia TCP
void Tcp_Open_Active(uint16_t port) {
	
	//Wysy³anie flagi Syn
	Tcp_Status = SendSyn;

	MakeIpHeader(0x06,0x14);
		
	EthFrame[SourcePort0] = (uint8_t)(port>>8);
	EthFrame[SourcePort1] = (uint8_t)(port);
		
	EthFrame[DestinPort0] = (uint8_t)(dest_port>>8);
	EthFrame[DestinPort1] = (uint8_t)(dest_port);
		
	EthFrame[SeqNumber0] = (uint8_t)(seq_number>>24); //Numer porz¹dkowy SEQ
	EthFrame[SeqNumber1] = (uint8_t)(seq_number>>16);
	EthFrame[SeqNumber2] = (uint8_t)(seq_number>>8);
	EthFrame[SeqNumber3] = (uint8_t)(seq_number);
		
	EthFrame[DataOffset] = 0x50;
	EthFrame[ControlBits] = SYN;
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
		
	if(EthFrame[DestinPort0] == (uint8_t)(dest_port>>8) && EthFrame[DestinPort1] == (uint8_t)(dest_port) 
		&& EthFrame[SourcePort0] == (uint8_t)(port>>8) && EthFrame[SourcePort1] == (uint8_t)(port)
		&& EthFrame[ControlBits] == (SYN|ACK) && Tcp_Status == SendSyn
		&& EthFrame[AckNumber3] == (uint8_t)(seq_number)+0x01) {
			
		//Przepisz ack_number
		ack_number  = (uint32_t)EthFrame[SeqNumber0]<<24;			//Numer potwierdzenia ACK
		ack_number += (uint32_t)EthFrame[SeqNumber1]<<16;
		ack_number += (uint32_t)EthFrame[SeqNumber2]<<8;	
		ack_number += (uint32_t)EthFrame[SeqNumber3];
		
		//Wysy³anie potwierdzenia
		Tcp_Status = Established;
		seq_number+=0x01;
		ack_number+=0x01;
		
		MakeIpHeader(0x06,0x14);
		
		EthFrame[SourcePort0] = (uint8_t)(port>>8);
		EthFrame[SourcePort1] = (uint8_t)(port);
		
		EthFrame[DestinPort0] = (uint8_t)(dest_port>>8);
		EthFrame[DestinPort1] = (uint8_t)(dest_port);
		
		EthFrame[SeqNumber0] = (uint8_t)(seq_number>>24); //Numer porz¹dkowy SEQ
		EthFrame[SeqNumber1] = (uint8_t)(seq_number>>16);
		EthFrame[SeqNumber2] = (uint8_t)(seq_number>>8);
		EthFrame[SeqNumber3] = (uint8_t)(seq_number);
		
		EthFrame[AckNumber0] = (uint8_t)(ack_number>>24);
		EthFrame[AckNumber1] = (uint8_t)(ack_number>>16);
		EthFrame[AckNumber2] = (uint8_t)(ack_number>>8);
		EthFrame[AckNumber3] = (uint8_t)(ack_number);
				
		EthFrame[DataOffset] = 0x50;
		EthFrame[ControlBits] = ACK;
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
		
		//Ustanowiono po³¹czenie
		Tcp_Status = Established;
	}	
}

//Pasywne zakoñczenie wirtualnego po³¹czenia TCP
void Tcp_Fin_Pasive(uint16_t port) {

	if(EthFrame[DestinPort0] == (uint8_t)(port>>8) && EthFrame[DestinPort1] == (uint8_t)(port) 
		&& EthFrame[ControlBits] == (FIN|ACK)) {
		Tcp_Status = CloseWait;

		MakeIpHeader(0x06,0x14);
		
		EthFrame[SourcePort0] = (uint8_t)(port>>8);
		EthFrame[SourcePort1] = (uint8_t)(port);
		
		EthFrame[DestinPort0] = (uint8_t)(dest_port>>8);
		EthFrame[DestinPort1] = (uint8_t)(dest_port);
		
		EthFrame[AckNumber0] = EthFrame[SeqNumber0];
		EthFrame[AckNumber1] = EthFrame[SeqNumber1];
		EthFrame[AckNumber2] = EthFrame[SeqNumber2];
		EthFrame[AckNumber3] = (EthFrame[SeqNumber3] + 1);
		
		EthFrame[SeqNumber0] = (uint8_t)(seq_number>>24); //Numer porz¹dkowy SEQ
		EthFrame[SeqNumber1] = (uint8_t)(seq_number>>16);
		EthFrame[SeqNumber2] = (uint8_t)(seq_number>>8);
		EthFrame[SeqNumber3] = (uint8_t)(seq_number);
		
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
		
		Tcp_Status = LastAck;
		//Czekanie na segment TCP z potwierdzeniem
		while(RTLreceive_packet()!=0x01) {}
		
		next_send_data=0;
		next_ack=0;

		if(EthFrame[DestinPort0] == (uint8_t)(port>>8) && EthFrame[DestinPort1] == (uint8_t)(port) 
			&& EthFrame[ControlBits] == ACK && EthFrame[AckNumber3] == (EthFrame[SeqNumber3]+0x01)) {
			//Zakoñczono po³¹czenie
			Tcp_Status = Closed;
		}
	}
}

//Aktywne zakoñczenie wirtualnego po³¹czenia TCP
void Tcp_Fin_Active(uint16_t port) {

	if(Tcp_Status == Established) {
	//Wysy³anie Fin i Ack
	Tcp_Status = FinWait1;
	MakeIpHeader(0x06,0x14);
			
	EthFrame[SourcePort0] = (uint8_t)(port>>8);
	EthFrame[SourcePort1] = (uint8_t)(port);
		
	EthFrame[DestinPort0] = (uint8_t)(dest_port>>8);
	EthFrame[DestinPort1] = (uint8_t)(dest_port);
		
	EthFrame[SeqNumber0] = (uint8_t)(seq_number>>24); //Numer porz¹dkowy SEQ
	EthFrame[SeqNumber1] = (uint8_t)(seq_number>>16);
	EthFrame[SeqNumber2] = (uint8_t)(seq_number>>8);
	EthFrame[SeqNumber3] = (uint8_t)(seq_number);
		
	EthFrame[DataOffset] = 0x50;
	EthFrame[ControlBits] = FIN|ACK;
	EthFrame[WindowSize] = 0xff;
		
	uint16_t CheckSum = TcpCheckSumCalc(0x00);
	EthFrame[TcpCheckSum0] = (uint8_t)((CheckSum)>>8);
	EthFrame[TcpCheckSum1] = (uint8_t)(CheckSum);
		
	EthFrame[DataPointer0] = 0x00;
	EthFrame[DataPointer1] = 0x00;
		
	//Wysy³anie do RTL8019as
	PacketSize = 14+20+20;
	RTLsend_packet();

	
	Tcp_Status = TimeWait;	
	//Czekanie na segment TCP z potwierdzeniem
	while(RTLreceive_packet()!=0x01) {}
	
	//Otrzymanie FIN i ACK	
	if(EthFrame[DestinPort0] == (uint8_t)(dest_port>>8) && EthFrame[DestinPort1] == (uint8_t)(dest_port) 
		&& EthFrame[SourcePort0] == (uint8_t)(port>>8) && EthFrame[SourcePort1] == (uint8_t)(port)
		&& EthFrame[ControlBits] == (/*FIN|*/ACK) && Tcp_Status == TimeWait
		&& EthFrame[AckNumber3] == (uint8_t)(seq_number)+0x01) {
		
		//Przepisz ack_number
		ack_number  = (uint32_t)EthFrame[SeqNumber0]<<24;			//Numer potwierdzenia ACK
		ack_number += (uint32_t)EthFrame[SeqNumber1]<<16;
		ack_number += (uint32_t)EthFrame[SeqNumber2]<<8;	
		ack_number += (uint32_t)EthFrame[SeqNumber3];
		
		//Wysy³anie potwierdzenia ACK
		Tcp_Status = Closing;
		ack_number+=0x01;
		
		MakeIpHeader(0x06,0x14);
		
		EthFrame[SourcePort0] = (uint8_t)(port>>8);
		EthFrame[SourcePort1] = (uint8_t)(port);
		
		EthFrame[DestinPort0] = (uint8_t)(dest_port>>8);
		EthFrame[DestinPort1] = (uint8_t)(dest_port);
		
		EthFrame[AckNumber0] = (uint8_t)(ack_number>>24);
		EthFrame[AckNumber1] = (uint8_t)(ack_number>>16);
		EthFrame[AckNumber2] = (uint8_t)(ack_number>>8);
		EthFrame[AckNumber3] = (uint8_t)(ack_number);
				
		EthFrame[DataOffset] = 0x50;
		EthFrame[ControlBits] = ACK;
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
		
		//Zakoñczono po³¹czenie
		Tcp_Status = Closed;
	}
	
	//Otrzymanie ACK
	if(EthFrame[DestinPort0] == (uint8_t)(dest_port>>8) && EthFrame[DestinPort1] == (uint8_t)(dest_port) 
		&& EthFrame[SourcePort0] == (uint8_t)(port>>8) && EthFrame[SourcePort1] == (uint8_t)(port)
		&& EthFrame[ControlBits] == ACK && Tcp_Status == TimeWait
		&& EthFrame[AckNumber3] == (uint8_t)(seq_number)+0x01) {
		
		//Wysy³anie potwierdzenia ACK
		Tcp_Status = FinWait2;
		
		while(EthFrame[ControlBits] == (FIN|ACK) && Tcp_Status == TimeWait
		&& EthFrame[AckNumber3] == (uint8_t)(seq_number)+0x01) 
		{
			//Czekanie na segment TCP z potwierdzeniem
			while(RTLreceive_packet()!=0x01) {}
		}
		
		//Przepisz ack_number
		ack_number  = (uint32_t)EthFrame[SeqNumber0]<<24;			//Numer potwierdzenia ACK
		ack_number += (uint32_t)EthFrame[SeqNumber1]<<16;
		ack_number += (uint32_t)EthFrame[SeqNumber2]<<8;	
		ack_number += (uint32_t)EthFrame[SeqNumber3];
		
		//Wysy³anie potwierdzenia ACK
		Tcp_Status = Closing;
		ack_number+=0x01;
		
		MakeIpHeader(0x06,0x14);
		
		EthFrame[SourcePort0] = (uint8_t)(port>>8);
		EthFrame[SourcePort1] = (uint8_t)(port);
		
		EthFrame[DestinPort0] = (uint8_t)(dest_port>>8);
		EthFrame[DestinPort1] = (uint8_t)(dest_port);
		
		EthFrame[AckNumber0] = (uint8_t)(ack_number>>24);
		EthFrame[AckNumber1] = (uint8_t)(ack_number>>16);
		EthFrame[AckNumber2] = (uint8_t)(ack_number>>8);
		EthFrame[AckNumber3] = (uint8_t)(ack_number);
				
		EthFrame[DataOffset] = 0x50;
		EthFrame[ControlBits] = ACK;
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
		
		//Zakoñczono po³¹czenie
		Tcp_Status = Closed;
	}
	}
}

//Funkcja sprawdzaj¹ca czy s¹ nowe dane TCP z konkretnego portu
uint8_t Tcp_Receive(uint16_t port) {

	if(EthFrame[DestinPort0] == (uint8_t)(port>>8) && EthFrame[DestinPort1] == (uint8_t)(port) 
		&& EthFrame[ControlBits] == (PSH+ACK) /*&& Tcp_Status == Established*/) {
		return 1; //Odebrano nowe dane
	}
	return 2; //B³¹d odbierania danych
}

//Funkcja wysy³aj¹ca potwierdzenie otrzymania danych TCP na konkretny destport
uint16_t Tcp_Send_Ack(uint16_t destport) {

		uint16_t receive_data = (EthFrame[IpTotalLength0]<<8); //iloœæ odebranych bajtów danych (20-nag³ówek IP, 20-nag³ówek TCP)
		receive_data += EthFrame[IpTotalLength1];
		receive_data -= (0x14+0x14);
		
		MakeIpHeader(0x06,0x14);
		
		EthFrame[DestinPort0] = EthFrame[SourcePort0];
		EthFrame[DestinPort1] = EthFrame[SourcePort1];
		
		EthFrame[SourcePort0] = (uint8_t)(destport>>8);
		EthFrame[SourcePort1] = (uint8_t)(destport);
		
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
void Tcp_Send_Data(uint16_t destport, uint16_t data_lenght) {	
	
	MakeIpHeader(0x06,0x14+data_lenght);
	
	EthFrame[DestinPort0] = (uint8_t)(dest_port>>8);
	EthFrame[DestinPort1] = (uint8_t)(dest_port);
	
	EthFrame[SourcePort0] = (uint8_t)(destport>>8);
	EthFrame[SourcePort1] = (uint8_t)(destport);
	
	EthFrame[SeqNumber0] = (uint8_t)(seq_number>>24); //Numer porz¹dkowy SEQ
	EthFrame[SeqNumber1] = (uint8_t)(seq_number>>16);
	EthFrame[SeqNumber2] = (uint8_t)(seq_number>>8);
	EthFrame[SeqNumber3] = (uint8_t)(seq_number);
	seq_number+=data_lenght;
	
	EthFrame[AckNumber0] = (uint8_t)(ack_number>>24); //Numer potwierdzenia ACK
	EthFrame[AckNumber1] = (uint8_t)(ack_number>>16);
	EthFrame[AckNumber2] = (uint8_t)(ack_number>>8);
	EthFrame[AckNumber3] = (uint8_t)(ack_number);
	
	EthFrame[DataOffset] = 0x50;
	EthFrame[ControlBits] = (/*PSH|*/ACK);
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

//Funkcja zamykaj¹ca po³¹czenie TCP na potrzeby HTTP
void Tcp_Fin_Http(uint16_t port) {
	printf("%x ", Tcp_Status);
	if(Tcp_Status == Established) {
	//Wysy³anie Fin i Ack
	Tcp_Status = FinWait1;
	MakeIpHeader(0x06,0x14);
			
	EthFrame[SourcePort0] = (uint8_t)(port>>8);
	EthFrame[SourcePort1] = (uint8_t)(port);
		
	EthFrame[DestinPort0] = (uint8_t)(dest_port>>8);
	EthFrame[DestinPort1] = (uint8_t)(dest_port);
		
	EthFrame[SeqNumber0] = (uint8_t)(seq_number>>24); //Numer porz¹dkowy SEQ
	EthFrame[SeqNumber1] = (uint8_t)(seq_number>>16);
	EthFrame[SeqNumber2] = (uint8_t)(seq_number>>8);
	EthFrame[SeqNumber3] = (uint8_t)(seq_number);
		
	EthFrame[DataOffset] = 0x50;
	EthFrame[ControlBits] = FIN|ACK;
	EthFrame[WindowSize] = 0xff;
		
	uint16_t CheckSum = TcpCheckSumCalc(0x00);
	EthFrame[TcpCheckSum0] = (uint8_t)((CheckSum)>>8);
	EthFrame[TcpCheckSum1] = (uint8_t)(CheckSum);
		
	EthFrame[DataPointer0] = 0x00;
	EthFrame[DataPointer1] = 0x00;
	printf("%x ", Tcp_Status);	
	//Wysy³anie do RTL8019as
	PacketSize = 14+20+20;
	RTLsend_packet();

	
	Tcp_Status = TimeWait;	
	//Czekanie na segment TCP z potwierdzeniem
	while(RTLreceive_packet()!=0x01) {}
	printf("%x ", Tcp_Status);
	//Otrzymanie ACK	
	if(EthFrame[DestinPort0] == (uint8_t)(dest_port>>8) && EthFrame[DestinPort1] == (uint8_t)(dest_port) 
		&& EthFrame[SourcePort0] == (uint8_t)(port>>8) && EthFrame[SourcePort1] == (uint8_t)(port)
		&& EthFrame[ControlBits] == (ACK) && Tcp_Status == TimeWait
		/*&& EthFrame[AckNumber3] == (uint8_t)(seq_number)+0x01*/) {
		
		//Przepisz ack_number
		ack_number  = (uint32_t)EthFrame[SeqNumber0]<<24;			//Numer potwierdzenia ACK
		ack_number += (uint32_t)EthFrame[SeqNumber1]<<16;
		ack_number += (uint32_t)EthFrame[SeqNumber2]<<8;	
		ack_number += (uint32_t)EthFrame[SeqNumber3];
		
		//Wysy³anie potwierdzenia FIN i ACK
		Tcp_Status = Closing;
		ack_number+=0x01;
		
		MakeIpHeader(0x06,0x14);
		
		EthFrame[SourcePort0] = (uint8_t)(port>>8);
		EthFrame[SourcePort1] = (uint8_t)(port);
		
		EthFrame[DestinPort0] = (uint8_t)(dest_port>>8);
		EthFrame[DestinPort1] = (uint8_t)(dest_port);
		
		EthFrame[AckNumber0] = (uint8_t)(ack_number>>24);
		EthFrame[AckNumber1] = (uint8_t)(ack_number>>16);
		EthFrame[AckNumber2] = (uint8_t)(ack_number>>8);
		EthFrame[AckNumber3] = (uint8_t)(ack_number);
				
		EthFrame[DataOffset] = 0x50;
		EthFrame[ControlBits] = FIN|ACK;
		//EthFrame[14] = 0x00;
		EthFrame[WindowSize] = 0xff;
		
		uint16_t CheckSum = TcpCheckSumCalc(0x00);
		EthFrame[TcpCheckSum0] = (uint8_t)((CheckSum)>>8);
		EthFrame[TcpCheckSum1] = (uint8_t)(CheckSum);
		
		EthFrame[DataPointer0] = 0x00;
		EthFrame[DataPointer1] = 0x00;
		printf("%x ", Tcp_Status);
		//Wysy³anie do RTL8019as
		PacketSize = 14+20+20;
		RTLsend_packet();
		
		//Otrzymanie ACK	
		if(EthFrame[DestinPort0] == (uint8_t)(dest_port>>8) && EthFrame[DestinPort1] == (uint8_t)(dest_port) 
			&& EthFrame[SourcePort0] == (uint8_t)(port>>8) && EthFrame[SourcePort1] == (uint8_t)(port)
			&& EthFrame[ControlBits] == (ACK) && Tcp_Status == TimeWait
			&& EthFrame[AckNumber3] == (uint8_t)(seq_number)+0x01) {
			//Zakoñczono po³¹czenie
			Tcp_Status = Closed;
		}
	}
	}
}
	
void Tcp_Check(uint16_t port) {
	
	//Nas³uch na porcie
	Tcp_Open_Pasive(port);
	//Zamkniêcie po³¹czenia na porcie 
	Tcp_Fin_Pasive(port);

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