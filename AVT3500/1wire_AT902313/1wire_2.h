#include <inttypes.h>
//makra
#define WE 1
#define PORT_1Wire PINB
#define SET_1Wire DDRB&=~_BV(WE)
#define CLEAR_1Wire DDRB|=_BV(WE)

char buf[8]; //zmienna potrzebna przy konwersji temperatury
uint8_t RESET_PULSE(void);
void send(uint8_t bit);
uint8_t read(void);
void send_byte(uint8_t wartosc);
uint8_t read_byte(void);