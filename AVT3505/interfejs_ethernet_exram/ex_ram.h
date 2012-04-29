#ifndef EX_RAM_H
#define EX_RAM_H

//definicja u³atwiaj¹ca korzystanie z zewnêtrznej pamiêci
#define EXMEM __attribute__ ((section (".exram")))

//funkcje
void strcopy(char* d, char* s);
int strleng(char *s);

#endif