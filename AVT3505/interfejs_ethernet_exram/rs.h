#ifndef RS_H_INCLUDED
#define RS_H_INCLUDED

//Definicja prêdkoœci UART-a
#define DEF_BAUD 9600

#define RS_MAKE_UBRR(baud) (F_CPU/(baud*16l)-1)
#define RS_SET_BAUD(baud) \
 { UBRR0H = (uint8_t)(RS_MAKE_UBRR(baud)>>8); \
   UBRR0L = (uint8_t)RS_MAKE_UBRR(baud); }

int rs_put(char znak);
int rs_get(void);

#endif //RS_H_INCLUDED
