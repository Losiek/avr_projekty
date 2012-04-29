#ifndef PAGE_DATA_H_INCLUDED
#define PAGE_DATA_H_INCLUDED

unsigned char content[] PROGMEM= "HTTP/1.0 200 OK\r\n Content-type: text/html\r\t\n";
unsigned char index1[] PROGMEM= "<HTML>\r\n<HEAD>\r\n<TITLE>Mikroserwer LAN</TITLE>\r\n"
"<meta http-equiv=\"content-type\" content=\"Text/Html; charset=windows-1250\">\r\n"
"</HEAD>\r\n"
"<BODY>\r\n"
//"<h1></br>Mikroserwer</h1>\r\n"
"</BODY>\r\n"
"</HTML>";

#endif //PAGE_DATA_H_INCLUDED