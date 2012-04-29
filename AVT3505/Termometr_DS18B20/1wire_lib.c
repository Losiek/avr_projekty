#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include "1wire_lib.h"

/*********************************************************************************/
// Reset interfejsu 1-Wire
//1-Wire reset
uint8_t reset_1w(void)
{
unsigned char presence=1;

	write_1w;
	write_zero;
	_delay_us(330);
	read_1w;
	_delay_us(70);
	if(clear_1w) presence=0;
	_delay_us(400);
	return presence;
}

/*********************************************************************************/
// Zapis bajtu
// Write byte to 1-wire device
void send_1w(unsigned char data) 
{
  unsigned char u;
  unsigned char x;
  for(u=0; u<8; u++)
  {
   x=0x01&data; 
   if (x==0) 
   {
     write_1w;
	 write_zero;
     _delay_us(60);
     read_1w;
     _delay_us(5);
   }
   else 
   {
     write_1w;
	 write_zero;
     _delay_us(5);
     read_1w;
     _delay_us(60);
   }
   data>>=1; 
  }
}

/*********************************************************************************/
// Odczyt bajtu
//Read byte from 1-Wire device
unsigned char get_1w(void) 
{
  unsigned char x=0x00;
  unsigned char u;
  for(u=0; u<8; u++)
  {
    x>>=1;
    write_1w;
	write_zero;
    _delay_us(4);
    read_1w;
    _delay_us(6);
    if(set_1w)
    {
      x=x+0x80;
    }
	_delay_us(90); 
  }
  return x;
}
