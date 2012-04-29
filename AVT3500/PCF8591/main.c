#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/pgmspace.h>

#include "harddef.h"
#include "delay.h"
#include "makra.h"
#include "i2c.h"
#include "lcd.h"

int main(void)
{
	//Inicjacja
	PORTD = 1<<I2C_SDA | 1<<I2C_SCL;
	DDRD = 1<<I2C_SCL;
	DDRB = 1<<LCD_E | 1<<LCD_RS | 0x0f<<LCD_D4;
	lcd_init();
	//Koniec inicjacji
	
	//Tekst informacyjny w górnej linii
	lcd_str_P((prog_char*)PSTR("Dane z I0:"));
	lcd_command(LCDC_DDA | 64);
	
	//Konfiguracja przetwornika
	i2c_start();
	//Bajt adresowy, zapis
	i2c_send(0x90);
	//bajt kontrolny
	i2c_send(0x00);
	i2c_stop();
	
	//Pobieranie danych
	for(;;)
	{
		lcd_command(LCDC_DDA | 64);
		//Odczyt danych
		i2c_start();
		//Bajt adresowy, odczyt
		i2c_send(0x91);
		//Pobieranie i wyświetlenie danej
		lcd_dec(i2c_get(I2C_NACK));
		//Przysłonięcie reszty napisu
		lcd_str_P((prog_char*)PSTR("  "));
		i2c_stop();
	}
	
	return 0;
}

