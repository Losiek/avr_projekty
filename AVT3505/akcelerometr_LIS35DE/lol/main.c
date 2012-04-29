//-----------------------------------------------------------------------------
//    KAmodMEMS2 software library
//-----------------------------------------------------------------------------
//
//    THE SOFTWARE INCLUDED IN THIS FILE IS FOR GUIDANCE ONLY.
//    BTC KORPORACJA SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
//    OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
//    FROM USE OF THIS SOFTWARE.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  This sample works with ZL15AVR evaluation board and 3 KAmodLED8 modules.
//
//  This program shows KAmodMEM2S angles in three axises on LED modules.
//  KAmodMEM2S generates interrupt INT1 when clicked, when ZL15AVR receives
//  interrupt, it plays sound.
//
//-----------------------------------------------------------------------------  
//  Connections:
//
//  KAmodLED8    |   ZL15AVR
//  ----------------------------
//  D0 (Con1)    |   PA0 (Con15)
//  D1 (Con1)    |   PA1 (Con15)
//  D2 (Con1)    |   PA2 (Con15)
//  D3 (Con1)    |   PA3 (Con15)
//  D4 (Con1)    |   PA4 (Con15)
//  D5 (Con1)    |   PA5 (Con15)
//  D6 (Con1)    |   PA6 (Con15)
//  D7 (Con1)    |   PA7 (Con15)
//  GND (Con1)   |   GND (Con15)
//  +5V (Con1)   |   +5V (Con15)
//
//
//  KAmodLED8    |   ZL15AVR
//  ----------------------------
//  D0 (Con1)    |   PC0 (Con17)
//  D1 (Con1)    |   PC1 (Con17)
//  D2 (Con1)    |   PC2 (Con17)
//  D3 (Con1)    |   PC3 (Con17)
//  D4 (Con1)    |   PC4 (Con17)
//  D5 (Con1)    |   PC5 (Con17)
//  D6 (Con1)    |   PC6 (Con17)
//  D7 (Con1)    |   PC7 (Con17)
//  GND (Con1)   |   GND (Con17)
//  +5V (Con1)   |   +5V (Con17)
//
//
//  KAmodLED8    |   ZL15AVR
//  ----------------------------
//  D0 (Con1)    |   PD0 (Con18)
//  D1 (Con1)    |   PD1 (Con18)
//  D2 (Con1)    |   PD2 (Con18)
//  D3 (Con1)    |   PD3 (Con18)
//  D4 (Con1)    |   PD4 (Con18)
//  D5 (Con1)    |   PD5 (Con18)
//  D6 (Con1)    |   PD6 (Con18)
//  D7 (Con1)    |   PD7 (Con18)
//  GND (Con1)   |   GND (Con18)
//  +5V (Con1)   |   +5V (Con18)
//
//
//  ZL15AVR	     |   ZL15AVR
//  ----------------------------
//  SPK (Con7)   |   PB0 (Con16)
//
//
//	ZL15AVR     		 	 |   KAmodMEMS2
//  -----------------------------------------
//  PD2 (Con17)         	 |   INT1 (Con1)
//  SS - PB4 (Pin 2, Con1)   |   SS (Con1)
//  MOSI - PB5 (Pin 3, Con1) |   MOSI (Con1)
//  MISO - PB6 (Pin 4, Con1) |   MISO (Con1)
//  SCK - PB7 (Pin 5, Con1)  |   SCK (Con1)
//  +5V (Pin 1, Con1)        |   +5V (Con1)
//  GND (Pin 6, Con1)        |   GND (Con1)
//  
//-----------------------------------------------------------------------------

#include <avr/interrupt.h>
#include <util/delay.h>
#include "lis35.h"


void Click()
{
	int i;
	for (i=1; i<5; i++)
	{
		if (PINB & 1) PORTB &= 0xFE; else PORTB |= 1;
		_delay_ms(10);
	}
} 

ISR(INT2_vect) 
{
	Click();
}


void InitializeLED(void)
{
	DDRA = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;	
}


void EnableInterrupts()
{
	DDRB &=	~(1 << PB2);
	PORTB = 0;
	MCUCSR |= 
	GIMSK=(1 << INT2);
	sei();
}



int main(void)
{
	signed char XPos, YPos, ZPos;
	signed int iXPos, iYPos, iZPos;

	InitializeLED();
	
	InitializeLIS35();

	// Set PB1 as output for speaker
	DDRB |= 1;


 

	if (InitializeLIS35()!=LIS35_OK)
	{
		while(1)
		{
			// When initialization fails indicate error with blinking LEDs
			PORTA = 0xFF;
			_delay_ms(100);
			PORTA = 0;
			_delay_ms(100);
		}
	}

	EnableInterrupts();

	while(1)
	{
		LIS35_GetPosition(&XPos, &YPos, &ZPos);

		//Convert angle values to LED number
		iXPos = (XPos + 64) >> 4;
		iYPos = (YPos + 64) >> 4;
		iZPos = (ZPos + 64) >> 4;
		
		PORTA = 1 << iXPos;
		PORTC = 1 << iYPos;
		PORTD = 1 << iZPos;		
	}
}

