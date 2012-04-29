#include <avr\io.h>
#include <avr\pgmspace.h>
#include <inttypes.h>
#include <stdlib.h>

#include "harddef.h"
#include "makra.h"
#include "delay.h"
#include "lcd.h"
//Funkcje niskego poziomu
#define LCD_EPULSE() \
	{PORT(LCD_EPORT) |= 1<<LCD_E; \
	delay250ns(); \
	PORT(LCD_EPORT) &= ~(1<<LCD_E);}

void lcd_sendHalf(uint8_t data)
{
	data = (data & 0x0F) << LCD_D4; 
	//data |= 0xF0;
	//data &= 0x0F;
	//data <<= LCD_D4; 
	PORT(LCD_DPORT) = 
		(PORT(LCD_DPORT) & ~(0x0F<<LCD_D4)) | data;
	//PORT(LCD_DPORT) &= ~(0x0F<<LCD_D4); 
	//PORT(LCD_DPORT) |= data; 
	LCD_EPULSE();
}

void lcd_send(uint8_t data)
{
	// Starsza czêœæ
	lcd_sendHalf(data>>4);
	// M³odsza czêœæ
	lcd_sendHalf(data);
	delayus8(120);
}

// Funkcje interfejsu
void lcd_command(uint8_t command)
{
	PORT(LCD_RSPORT) &= ~(1<<LCD_RS);
	lcd_send(command);
}

void lcd_data(uint8_t data)
{
	PORT(LCD_RSPORT) |= 1<<LCD_RS;
	lcd_send(data);
}

void lcd_cls(void)
{
	lcd_command(LCDC_CLS);
	delay100us8(48);
}

void lcd_home(void)
{
	lcd_command(LCDC_HOME);
	delay100us8(48);
}

void lcd_init(void)
{
	delay100us8(150);
	PORT(LCD_RSPORT) &= ~(1<<LCD_RS);
	lcd_sendHalf(LCDC_FUNC|LCDC_FUNC8b); 
	delay100us8(41);
	lcd_sendHalf(LCDC_FUNC|LCDC_FUNC8b);
	delay100us8(2);
	lcd_sendHalf(LCDC_FUNC|LCDC_FUNC4b);
	delay100us8(2);
	// Teraz jest ju¿ 4b. Koniec korzystania z sendHalf
	lcd_command(LCDC_FUNC|LCDC_FUNC4b|LCDC_FUNC2L|LCDC_FUNC5x7);
	lcd_command(LCDC_ON);
	lcd_cls();
	lcd_command(LCDC_MODE|LCDC_MODER);
	lcd_command(LCDC_ON|LCDC_ONDISPLAY|LCDC_ONCURSOR);
}

// Koniec obs³ugi wyœwietlacza
//--------------------------------------------------

//Funkcje pisz¹ce
void lcd_str_P(prog_char* str)
{
	char znak;
	while(0 != (znak=pgm_read_byte(str++)))
		lcd_data(znak);
}

void lcd_str(char* str)
{
	char znak;
	while(0 != (znak = *(str++)))
		lcd_data(znak);
}

//Wypisuje liczbê na wyœwietlaczu
void lcd_dec(int val)
{
	char bufor[7];
	lcd_str(itoa(val, bufor, 10));
}
