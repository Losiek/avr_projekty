#include <avr/io.h>
#include <avr/interrupt.h> //Blokowanie przerwañ na wszelki wypadek
#include <harddef.h>
#include <inttypes.h>

// procedura opóŸniaj¹ca o (5+5*t)* 0,25 us dla 4MHz kwarcu
void delay(uint8_t t)
{
do{asm("nop");}while(--t);
}
// procedura reset
void ow_reset(void)
{
DDRD |= 1<<DQ;
CLR_DQ; // stan niski na linii 1wire
delay(255); //
delay(119); // opóŸnienie ok 480us
//delay(1);
SET_DQ;// stan wysoki na linii 1wire
delay(255); //
delay(119); // opóŸnienie ok 480 us
}
// procedura zapisu bitu na liniê 1wire
void ow_write_bit(uint8_t b)
{
DDRD |= 1<<DQ;
cli(); // zablokowanie przerwañ
CLR_DQ; // stan niski na linii 1wire
delay(7); // opóŸnienie 10us
if(b) SET_DQ; // jeœli parametr jest niezerowy to ustaw stan wysoki na linii
delay(80); // opóŸnienie 100us
SET_DQ; // stan wysoki na linii 1wire
sei(); // odblokowanie przerwañ
}

uint8_t ow_read_bit(void)
{
cli();
DDRD &= ~(1<<DQ);
CLR_DQ;
delay(1);
SET_DQ;
delay(11);
sei();
if(IN_DQ) return 1; else return 0;
}

uint8_t ow_read_byte(void)
{
uint8_t i;
uint8_t value = 0;
for (i=0;i<8;i++)
{
if(ow_read_bit()) value|=0x01<<i;
delay(6);
}
return(value);
}

void ow_write_byte(char val)
{
unsigned char i;
unsigned char temp;
for (i=0; i<8; i++)
{
temp = val >> i;
temp &= 0x01;
ow_write_bit(temp);
}
delay(5);
}
