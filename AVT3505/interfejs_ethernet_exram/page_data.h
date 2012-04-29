#ifndef PAGE_DATA_H_INCLUDED
#define PAGE_DATA_H_INCLUDED

#include "ex_ram.h"

prog_uchar content[] = "HTTP/1.0 200 OK\r\n Content-type: text/html\r\t\n";
prog_uchar index1[] = "<HTML>\r\n<HEAD>\r\n<TITLE>Mikroserwer LAN</TITLE>\r\n"
"<meta http-equiv=\"content-type\" content=\"Text/Html; charset=windows-1250\">\r\n"
"</HEAD>\r\n"
"<BODY>\r\n"
"<h1></br>Mikro</h1>\r\n"
"</BODY>\r\n"
"</HTML>";

/*prog_uchar index2[] = "<head>"
"<meta http-equiv=""Content-Type"" content=""text/html; charset=iso-8859-2"" />"
"<meta name=""Description"" content=""Strona testowa"" />"
"<meta name=""Keywords"" content=""Serwer, WWW"" />"
"<title>Strona testowa</title>"
"</head>"
"<body>"
"<p>Przykładowa strona testowa</p>"
"</body>"
"</html>";*/
#endif //PAGE_DATA_H_INCLUDED