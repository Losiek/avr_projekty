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



#include "lis35.h"
#include <avr/io.h>
#include <util/delay.h>



/************************* SPI Functions **********************************************/
void SPI_Init(void)
{
	
	DDR_SPI |= (1<<DD_MOSI)|(1<<DD_SCK);
	DDR_SPI &= ~(1 << DD_MISO)|(1<<DD_SS);
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPOL)|(1<<CPHA) | (1<<DORD)|(1<<SPR0)|(1<<SPR1);
	SPSR = 1<<SPI2X;
}


void SPI_Transmit(char cData)
{
	SPDR = cData;
	while(!(SPSR & (1<<SPIF)));
}


char SPI_Receive(void)
{
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}
/************************* SPI Functions - end *****************************************/





void LIS35_RebootMemory()
{
	PORTB&=!(1<<DD_SS);
	SPI_Transmit(LIS35_WRITE|LIS35_ADDR_NO_INC|LIS35_REG_CR2);
	SPI_Transmit(LIS35_REG_CR2_BOOT);
	PORTB|=(1<<DD_SS);
}


char InitializeLIS35()
{
	char RegVal;

	//Inititalize SPI
	SPI_Init();

	//Reset LIS35 configuration
    LIS35_RebootMemory(); 

	//Power up
	LIS35_WriteRegister(LIS35_REG_CR1, LIS35_REG_CR1_XEN | LIS35_REG_CR1_YEN | LIS35_REG_CR1_ZEN | LIS35_REG_CR1_ACTIVE);// | LIS35_REG_CR1_FULL_SCALE);
	LIS35_ReadRegister(LIS35_REG_CR1, &RegVal);



	//Configure click interrupt (enable all sigle clicks)
	LIS35_WriteRegister(LIS35_CLICK_CFG, 0x1);
	LIS35_WriteRegister(LIS35_CLICK_THSY_X, 0x77);
	LIS35_WriteRegister(LIS35_CLICK_THSZ, 0x7);
	LIS35_WriteRegister(LIS35_CLICK_TIME_LIMIT, 0xFF);
	
	//Enable interrupts
	// LIS35 int 1 - click
	// LIS32_CR3_IHL - interrupt is low pulse
	 LIS35_WriteRegister(LIS35_CR3, LIS35_CR3_IHL | LIS35_CR3_CLICK_INT);



	//Check whether power up is set
	if (RegVal==(LIS35_REG_CR1_XEN | LIS35_REG_CR1_YEN | LIS35_REG_CR1_ZEN | LIS35_REG_CR1_ACTIVE  /*| LIS35_REG_CR1_FULL_SCALE*/)) return LIS35_OK;

	return LIS35_ERROR;
}


void LIS35_ReadRegister(char addr,char * v)
{
	PORTB&=!(1<<DD_SS);
	SPI_Transmit(LIS35_READ|LIS35_ADDR_NO_INC|addr);
	*v=SPI_Receive();
	PORTB|=(1<<DD_SS);
}


void LIS35_WriteRegister(char addr,char v)
{
	PORTB&=!(1<<DD_SS);
	SPI_Transmit(LIS35_WRITE|LIS35_ADDR_NO_INC|addr);
	SPI_Transmit(v);
	PORTB|=(1<<DD_SS);
}

void LIS35_GetPosition(signed char * x, signed char * y, signed char * z)
{
	PORTB&=!(1<<DD_SS);
	SPI_Transmit(LIS35_READ|LIS35_ADDR_INC|LIS35_REG_OUTX);
	*x=SPI_Receive();
	SPI_Receive();
	*y=SPI_Receive();
	SPI_Receive();
	*z=SPI_Receive();
	PORTB|=(1<<DD_SS);
}

