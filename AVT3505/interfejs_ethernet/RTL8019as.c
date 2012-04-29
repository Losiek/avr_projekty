//Funkcje obs³uguj¹ce RTL8019as

#include <avr/io.h>
#include <util/delay.h>

#include "RTL8019as.h"

//konfiguracja linii portów mikrokontrolera
void RTLsetup_ports(void) {
	
	RTL_DDR = 0xFF;
	RTL_PORT = 0xFF;
	
	RTL_CONTROL_DDR |= (1<<RTL_CONTROL_READPIN);
	RTL_CONTROL_DDR |= (1<<RTL_CONTROL_WRITEPIN);
	RTL_CONTROL_DDR |= (1<<RTL_CONTROL_AEN);
	RTL_CONTROL_PORT |= (1<<RTL_CONTROL_READPIN);
	RTL_CONTROL_PORT |= (1<<RTL_CONTROL_WRITEPIN);
	RTL_CONTROL_PORT |= (1<<RTL_CONTROL_AEN);
	
	RTL_RESET_DDR |= (1<<RTL_RESET_PIN);
}
//konic konfiguracji

//Reset kontrolera sieci
void RTLhw_reset(void) {
	
	RTL_RESET_PORT |= (1<<RTL_RESET_PIN);
	_delay_ms(10);
	RTL_RESET_PORT &= ~(1<<RTL_RESET_PIN);
	_delay_ms(10); 
}
//koniec funkcji resetuj¹cej kontroler

//Zapis bajtu do RTL8019as
void RTLwrite (uint8_t address, uint8_t data) {
	
	RTL_DDR = 0xff;
	//RTL_CONTROL_PORT = ~(1<<RTL_CONTROL_AEN);
	RTL_PORT = address;
	PORTE = ~(1<<PORTE1);
	RTL_PORT = data;
	
	RTL_CLEAR_WRITE;
	RTL_SET_WRITE;
	
	RTL_CONTROL_PORT = (1<<RTL_CONTROL_AEN);
	PORTE = 1<<PORTE1;
	
	RTL_DDR = 0x00;
	RTL_PORT = 0xFF;
}
//Koniec funkcji zapisuj¹cej bajt

//Funkcja odczytuj¹ca jeden bajt z RTL8019as
uint8_t RTLread(uint8_t address) {
	
	uint8_t byte;
	RTL_DDR = 0xff;
	RTL_CONTROL_PORT = ~(1<<RTL_CONTROL_AEN);
	RTL_PORT = address;
	PORTE = ~(1<<PORTE1);
	RTL_DDR = 0x00;
	asm volatile("nop\n\t"::);			//Gdy czêstoœæ 8MHz wystarczy jeden nop
	asm volatile("nop\n\t"::);
	
	RTL_CLEAR_READ;
	asm volatile("nop\n\t"::);
	asm volatile("nop\n\t"::);
	
	byte = RTL_PIN;
	
	PORTE = 1<<PORTE1;
	RTL_SET_READ;
	RTL_CONTROL_PORT = (1<<RTL_CONTROL_AEN);
	
	return byte;

}
//koniec funkcji odczytuj¹cej

//Inicjalizacja RTL8019as
void RTLinit(void) {

	
	RTLsetup_ports();
	RTLhw_reset();
	RTLwrite(RSTPORT,0);
	_delay_ms(30);
	
	RTLwrite(CR, 0x41);
	_delay_ms(2);
	
	RTLwrite(CURR, RXSTART_INIT);
	RTLwrite(PAR0+0, MyMac[0]);	//Zapis adresu MAC
	RTLwrite(PAR0+1, MyMac[1]);
	RTLwrite(PAR0+2, MyMac[2]);
	RTLwrite(PAR0+3, MyMac[3]);
	RTLwrite(PAR0+4, MyMac[4]);
	RTLwrite(PAR0+5, MyMac[5]);
	
	RTLwrite(CR,0x21);
	_delay_ms(2);
	
	RTLwrite(DCR, 0x58);
	
	RTLwrite(RCR, 0x04);
	
	RTLwrite(TCR, 0x02);
	
	RTLwrite(RBCR0, 0x00);
	RTLwrite(RBCR1, 0x00);
	RTLwrite(TPSR, TXSTART_INIT);
	RTLwrite(PSTART, RXSTART_INIT);
	RTLwrite(BNRY, RXSTART_INIT);
	RTLwrite(PSTOP, RXSTOP_INIT);
	
	RTLwrite(ISR, 0xFF);
	RTLwrite(IMR, 0x11);
	
	RTLwrite(TCR, 0x00);
	RTLwrite(CR, 0x22);
}
//koniec pe³nej inicjalizacji kontrolera

//Wysy³anie pakietu do RTL8019as
void RTLsend_packet(void) {

	uint8_t i;
	
	RTLwrite(CR, 0x22);
	
	while(RTLread(CR) & 0x04) {}; //czekanie a¿ mo¿na wys³aæ
	
	RTLwrite(TPSR,TXSTART_INIT);
	
	RTLwrite(RSAR0, 0x00);
	RTLwrite(RSAR1, TXSTART_INIT);
	
	RTLwrite(ISR, (1<<ISR_PTX));
	
	RTLwrite(RBCR0, 0xEA);
	RTLwrite(RBCR1, 0x05);
	
	RTLwrite(CR, 0x12);
	
	for(i=0; i<PacketSize; i++) {
		
		RTLwrite(RDMAPORT, EthFrame[i]);	//zapis ramki do kontrolera
	}
	
	while (PacketSize<60) {
		
		RTLwrite(RDMAPORT, 0);	//Je¿eli ramka mnijsza ni¿ 60 bajtów to dope³nij zerami
		PacketSize++;
	}
	
	RTLwrite(TBCR0, (uint8_t)(PacketSize));	//zapisz rozmiar ramiki
	RTLwrite(TBCR1, (uint8_t)((PacketSize)>>8));

	RTLwrite(CR, 0x24);	//wys³anie ramki przez sieæ
	
	RTLwrite(ISR, (1<<ISR_RDC));
}// koniec funkcju wysy³aj¹cej pakiet

//Funkcja sprawdzaj¹ca czy w buforze kontrolera jest nowy pakiet
uint8_t RTLreceive_empty_check(void) {
	
	uint8_t curr;
	
	RTLwrite(CR, 0x68);
	curr = RTLread(CURR);
	
	RTLwrite(CR, 0x22);
	
	return (curr == RTLread(BNRY)); //Je¿eli wskaŸniki równe to znaczy ¿e nie odebrano pakietu
}// koniec funkcji sprawdzaj¹cej bufor

//Funkcja koñcz¹ca odczyt ramki z kontrolera
void RTLend_retreive(void) {
	uint8_t i;
	
	RTLwrite(CR, 0x22);
	for(i=0; i <= 20; i++) {
		if(RTLread(ISR) & 1<<6) {
			break;
		}
	}
	RTLwrite(ISR, 1<<6);
		
	RTLwrite(BNRY, nextPacketPtr);
}// koniec funkcji koncz¹cej odczyt

//Funkcja odczytuj¹ca ramkê z kontrolera sieci
uint8_t RTLreceive_packet(void) {
	
	uint8_t i;
	
	if(RTLreceive_empty_check()) return 2;
	
	RTLwrite(CR, 0x22);
	
	currentPacketPtr = RTLread(BNRY);
	currentRetreiveAddress = (currentPacketPtr<<8) + 4;
	
	RTLwrite(ISR, (1<<ISR_PRX));
	
	if ((currentPacketPtr >= RXSTOP_INIT) || (currentPacketPtr < RXSTART_INIT)) {
		
		RTLwrite(BNRY, RXSTART_INIT);
		RTLwrite(CR, 0x62);
		RTLwrite(CURR, RXSTART_INIT);
		RTLwrite(CR, 0x22);
		return 3;
	}
	
	RTLwrite(RBCR0, 4+ETHERNET_HEADER_LENGTH);
	RTLwrite(RBCR1, 0);
	RTLwrite(RSAR0, 0);
	RTLwrite(RSAR1, currentPacketPtr);
	
		RTLwrite(CR, 0x0A);
		
		RTLread(RDMAPORT);
		nextPacketPtr = RTLread(RDMAPORT);
		
		if((nextPacketPtr >= RXSTOP_INIT) || (nextPacketPtr < RXSTART_INIT)) return 4;
		
		PacketSize = RTLread(RDMAPORT);
		PacketSize |= (uint8_t)RTLread(RDMAPORT);
		
		if(PacketSize > 4) {
			PacketSize -= 4;
		} else {
			RTLend_retreive();
			return 5;
		}
		
		if(PacketSize > 1514) {
			RTLend_retreive();
			return 6;
		}
		
		EthFrame[EthDestMac+0]=RTLread(RDMAPORT);
		EthFrame[EthDestMac+1]=RTLread(RDMAPORT);
		EthFrame[EthDestMac+2]=RTLread(RDMAPORT);
		EthFrame[EthDestMac+3]=RTLread(RDMAPORT);
		EthFrame[EthDestMac+4]=RTLread(RDMAPORT);
		EthFrame[EthDestMac+5]=RTLread(RDMAPORT);
		
		EthFrame[EthSourceMac+0]=RTLread(RDMAPORT);
		EthFrame[EthSourceMac+1]=RTLread(RDMAPORT);
		EthFrame[EthSourceMac+2]=RTLread(RDMAPORT);
		EthFrame[EthSourceMac+3]=RTLread(RDMAPORT);
		EthFrame[EthSourceMac+4]=RTLread(RDMAPORT);
		EthFrame[EthSourceMac+5]=RTLread(RDMAPORT);
		
		EthFrame[EthTypeLen+0]=RTLread(RDMAPORT);
		EthFrame[EthTypeLen+1]=RTLread(RDMAPORT);
		
		currentRetreiveAddress += 6+6+2;
		
	RTLwrite(CR, 0x22);
	for(i=0; i <= 20; i++) 
		if(RTLread(ISR) & 1<<6) 
			break;
	RTLwrite(ISR, 1<<6);
		
	RTLwrite(RBCR0, (uint8_t)PacketSize);
	RTLwrite(RBCR1, (uint8_t)(PacketSize>>8));
	RTLwrite(RSAR0, (uint8_t)currentRetreiveAddress);
	RTLwrite(RSAR1, (uint8_t)(currentRetreiveAddress>>8));
	
	RTLwrite(CR, 0x0A);
	
		for(i=0; i < PacketSize; i++) 
			EthFrame[14+i] = RTLread(RDMAPORT);
			
		currentRetreiveAddress += PacketSize;
		if( currentRetreiveAddress >= 0x6000) 
			currentRetreiveAddress = currentRetreiveAddress - (0x6000-0x4600);	
	
	RTLend_retreive();		
	return 1;
}// koniec funkcji odczytuj¹cej ramkê

//Funkcja sprawdzaj¹ca czy odebrana ramka to ARP
uint8_t PacketCheck(void) {

	if(EthFrame[EthTypeLen+0]==0x08 && EthFrame[EthTypeLen+1]==0x06) {
		return 1; //wiadomoœæ ARP
	}
	
	if(EthFrame[EthTypeLen+0]==0x08 && EthFrame[EthTypeLen+1]==0x00) {
		return 2; //pakiet IP
	}
	
	return 0;
}// koniec funkcji sprawdzj¹cej odebran¹ ramkê


//Funkcja tworz¹c nag³ówek ramki Ethernet
void MakeEthHeader(uint8_t dest0, uint8_t dest1, uint8_t dest2, uint8_t dest3, uint8_t dest4, uint8_t dest5, uint8_t type0, uint8_t type1)
{
	PacketSize=0;
	
	EthFrame[EthDestMac+0]=dest0;
	EthFrame[EthDestMac+1]=dest1;
	EthFrame[EthDestMac+2]=dest2;
	EthFrame[EthDestMac+3]=dest3;
	EthFrame[EthDestMac+4]=dest4;
	EthFrame[EthDestMac+5]=dest5;
	
	EthFrame[EthSourceMac+0]=MyMac[0];
	EthFrame[EthSourceMac+1]=MyMac[1];
	EthFrame[EthSourceMac+2]=MyMac[2];
	EthFrame[EthSourceMac+3]=MyMac[3];
	EthFrame[EthSourceMac+4]=MyMac[4];
	EthFrame[EthSourceMac+5]=MyMac[5];
	
	EthFrame[EthTypeLen+0]=type0;
	EthFrame[EthTypeLen+1]=type1;
	
	PacketSize=PacketSize+14;

}//Koniec funkcji tworz¹cej nag³ówek
