#include <inttypes.h>
#include <util/delay.h>
//makra
#define WE 0
#define PORT_1Wire PINB
#define SET_1Wire DDRB&=~_BV(WE)
#define CLEAR_1Wire DDRB|=_BV(WE)

unsigned char bufor1w[8]; //bufor danych przychodz¹cych na magistralê 1-Wire.
unsigned char *pbufor1w; //wskaŸnik na bufor danych przychodz¹cych na magistralê 1-Wire.

//Deklaracje zmiennych
uint8_t RESET_PULSE(void);
void send(uint8_t bit);
uint8_t read(void);
void send_byte(uint8_t wartosc);
uint8_t read_byte(void);

//Deklaracje funkcji
void  SEARCH_ROM(void);
void  READ_ROM(void);
void  MATCH_ROM(void);
void  SKIP_ROM(void);