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
#include "i2c.h"


void LIS35_RebootMemory()
{
	i2c_send(0x38); //Adres i zapis
	i2c_send(LIS35_WRITE|LIS35_ADDR_NO_INC|LIS35_REG_CR2);
	i2c_send(LIS35_REG_CR2_BOOT);
	i2c_stop();
}


char InitializeLIS35()
{
	char RegVal;

	//Inititalize I2C
	i2c_start();

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
	i2c_start();
	i2c_send(0x39); //Adres i odczyt
	i2c_send(LIS35_READ|LIS35_ADDR_NO_INC|addr);
	*v=i2c_get(I2C_ACK);
	i2c_stop();
}


void LIS35_WriteRegister(char addr,char v)
{
	i2c_start();
	i2c_send(0x38); //Adres i zapis
	i2c_send(LIS35_WRITE|LIS35_ADDR_NO_INC|addr);
	i2c_send(v);
	i2c_stop();
}

void LIS35_GetPosition(signed char * x, signed char * y, signed char * z)
{
	i2c_start();
	i2c_send(0x39); //Adres i odczyt
	i2c_send(LIS35_READ|LIS35_ADDR_INC|LIS35_REG_OUTX);
	*x=i2c_get(I2C_ACK);
	i2c_get(I2C_ACK);
	*y=i2c_get(I2C_ACK);
	i2c_get(I2C_ACK);
	*z=i2c_get(I2C_ACK);
	i2c_stop();
}

