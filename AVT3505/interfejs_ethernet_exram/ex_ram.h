#ifndef EX_RAM_H
#define EX_RAM_H

//definicja u�atwiaj�ca korzystanie z zewn�trznej pami�ci
#define EXMEM __attribute__ ((section (".exram")))

//funkcje
void strcopy(char* d, char* s);
int strleng(char *s);

#endif