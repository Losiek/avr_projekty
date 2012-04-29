#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "ex_ram.h"

#include "protocols/_all_protocols.h"
#include "page_data.h"

#include "RTL8019as.h"

void Http_Check(void) {
	//while(RTLreceive_packet()!=0x01) {}
	if(EthFrame[HttpData+0] == 'G' && EthFrame[HttpData+1] == 'E' && EthFrame[HttpData+2] == 'T'
		&& Tcp_Status == Established) {
		Tcp_Send_Ack(0x1F90);
		
		strcpy_P(&EthFrame[14+20+20],content);
		Tcp_Send_Data(0x1F90, strlen_P(content));
		
		strcpy_P(&EthFrame[14+20+20],index1);
		Tcp_Send_Data(0x1F90, strlen_P(index1));
		
		Tcp_Fin_Http(0x1F90);
	}
}
	