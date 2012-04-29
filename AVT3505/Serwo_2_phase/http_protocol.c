#include "tcp_protocol.h"
#include "ip_protocol.h"
#include "arp_protocol.h"
#include "RTL8019as.h"
#include <util/delay.h>
#include "page_data.h"
#include "http_protocol.h"

void Http_Check(void) {
	if(Tcp_Receive(0x50) == 1) {
		if(EthFrame[HttpData+0] == 'G' && EthFrame[HttpData+1] == 'E' && EthFrame[HttpData+2] == 'T') {
			Tcp_Send_Ack(0x50);
		
			strncpy_P(&EthFrame[14+20+20],content,strlen_P(content));
			strncpy_P(&EthFrame[14+20+20+strlen_P(content)],index,strlen_P(index));
		
			Tcp_Send_Data(0x50, strlen_P(content)+strlen_P(index));
		}
	}
}
	