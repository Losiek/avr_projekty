#ifndef HTTP_PROTOCOL_H_INCLUDED
#define HTTP_PROTOCOL_H_INCLUDED

#define EthData 14
#define HttpData 14+20+20 

//Status TCP
uint8_t Tcp_Status;

void Http_Check(void);

#endif //HTTP_PROTOCOL_H_INCLUDED