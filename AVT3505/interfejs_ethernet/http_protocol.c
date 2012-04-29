#include "tcp_protocol.h"
#include "ip_protocol.h"
#include "arp_protocol.h"
#include "RTL8019as.h"
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "page_data.h"
#include "http_protocol.h"

void Http_Check(void) {
	//while(RTLreceive_packet()!=0x01) {}
	if(EthFrame[HttpData+0] == 'G' && EthFrame[HttpData+1] == 'E' && EthFrame[HttpData+2] == 'T'
		/*&& Tcp_Status == Established*/) {
		Tcp_Send_Ack(0x1F90);
		
		strncpy_P(&EthFrame[14+20+20],content,strlen_P(content));
		Tcp_Send_Data(0x1F90, strlen_P(content));
		
		/*strncpy_P(&EthFrame[14+20+20],index1,strlen_P(index1));
		Tcp_Send_Data(0x1F90, strlen_P(index1));*/
		
		//Tcp_Fin_Active(0x1F90);
	}
}
	