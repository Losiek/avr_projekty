#ifndef EX_RAM_H
#define EX_RAM_H

//definicja ułatwiająca korzystanie z zewnętrznej pamięci
#define EXMEM __attribute__ ((section (".exram")))

//funkcje
void strcopy(char* d, char* s);
int strleng(char *s);

#endif