//OpóŸnienie po³ówkowe
#include <avr/io.h>
#include <inttypes.h>

#include "harddef.h"
#include "makra.h"
#include "i2c.h"

#define I2C_nhalf (F_CPU/I2C_SPEED/2)

//Funkcja d³ó¿szych opóŸnieñ
#if I2C_nhalf < 3
	//Nic
#elif I2C_nhalf < 8
	static void i2c_xdelay(void)
	{
		NOP();
	}
#else
	#define I2C_delayloops (1+(I2C_nhalf-8)/3)
	#if I2C_delayloops > 255
		#error Przyspiesz - bo sie nie wyrabiam ;)
	#endif
	static void i2c_xdelay(void)
	{
		asm volatile( \
			"delayus8_loop%=: \n\t"\
			"dec %[ticks] \n\t"\
			"brne delayus8_loop%= \n\t"\
		: :[ticks]"r"(I2C_delayloops) );
	}
#endif//I2C_nhalf >= 3

//OpóŸnienia dla I2C
static inline void i2c_hdelay(void)
{
#if I2C_nhalf < 1
	return;
#elif I2C_nhalf < 2
	NOP();
#elif I2C_nhalf < 3
	asm volatile(
		"rjmp exit%=\n\t"
		"exit%=:\n\t"::);
#else
	i2c_xdelay();
#endif
}

//Ustawienia i zerowanie wyjœcia
static inline void i2c_sdaset(void)
{
	DDR(I2C_SDAPORT) &= ~(1<<I2C_SDA);
	PORT(I2C_SDAPORT) |= 1<<I2C_SDA;
}

static inline void i2c_sdaclear(void)
{
	PORT(I2C_SDAPORT) &= ~(1<<I2C_SDA);
	DDR(I2C_SDAPORT) |= 1<<I2C_SDA;
}

//Pobieranie danej z wyprowadzenia portu
static inline uint8_t i2c_sdaget(void)
{
	return PIN(I2C_SDAPORT) & (1<<I2C_SDA);
}

//Zerowanie i ustawianie zegara
static inline void i2c_sclset(void)
{
	PORT(I2C_SCLPORT) |= 1<<I2C_SCL;
}

static inline void i2c_sclclear(void)
{
	PORT(I2C_SCLPORT) &= ~(1<<I2C_SCL);
}

void i2c_start(void)
{
	//Jeœli start bez stop
	i2c_sdaset();
	i2c_hdelay();
	i2c_sclset();
	i2c_hdelay();
	//Normalna sekwencja startu
	i2c_sdaclear();
	i2c_hdelay();
	i2c_sclclear();
}

void i2c_stop(void)
{
	i2c_sdaclear();
	i2c_hdelay();
	i2c_sclset();
	i2c_hdelay();
	i2c_sdaset();
	i2c_hdelay();
}

uint8_t i2c_send(uint8_t data)
{
	uint8_t n;
	
	for(n=8; n>0; --n)
	{
		if(data & 0x80)
			i2c_sdaset();
		else
			i2c_sdaclear();
		data <<= 1;
		i2c_hdelay();
		i2c_sclset();
		i2c_hdelay();
		i2c_sclclear();
	}
	//ACK potwierdzenie transmisji
	i2c_sdaset();
	i2c_hdelay();
	i2c_sclset();
	i2c_hdelay();
	n = i2c_sdaget();
	i2c_sclclear();
	
	return n;
}

uint8_t i2c_get(uint8_t ack)
{
	uint8_t n, temp=0;
	
	i2c_sdaset();
	for(n=8; n>0; --n)
	{
		i2c_hdelay();
		i2c_sclset();
		i2c_hdelay();
		temp<<=1;
		if(i2c_sdaget())
			temp++;
		i2c_sclclear();
	}
	//ACK
	if(ack == I2C_ACK)
		i2c_sdaclear();
	else
		i2c_sdaset();
	i2c_hdelay();
	i2c_sclset();
	i2c_hdelay();
	i2c_sclclear();
	
	return temp;
}
