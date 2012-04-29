//delay.c - funkcje opóŸnieñ
#include <avr/io.h>
#include "delay.h"

void delay100us8(uint8_t t)
{
	while (t>0)
	{
		delayus8(100);
		--t;
	}
}
