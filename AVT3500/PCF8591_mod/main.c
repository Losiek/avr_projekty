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
	
	uint8_t out = 155;
	uint8_t I0, I1, I2, I3;
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
		//Zapis danych
		i2c_start();
		//Bajt adresowy, zapis
		i2c_send(0x90);
		//Bajt konfiguracyjny D\A i A\D
		i2c_send(0x44);
		//Wys³anie danych do D\A
		i2c_send(out);//powoduje zak³ócenia
		//Wyœwietlenie danych z OUT
		//lcd_dec(out);
		//Przys³oniêcie reszty napisu
		//lcd_str_P((prog_char*)PSTR("  "));
		i2c_stop();
		
		//Odbiór danych
		i2c_start();
		//Bajt adresowy, Odbiór
		i2c_send(0x91);
		//Odbieranie danych z A\D do odpowiednich zmiennych
		I0 = i2c_get(I2C_ACK);
		I1 = i2c_get(I2C_ACK);
		I2 = i2c_get(I2C_ACK);
		I3 = i2c_get(I2C_NACK);
		i2c_stop();
		
		//Wyœwietlanie danych z kana³u I0
		lcd_dec(I0);
		//Przys³oniêcie reszty napisu
		lcd_str_P((prog_char*)PSTR("  "));
	}
	
	return 0;
}

