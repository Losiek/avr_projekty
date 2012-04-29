#include <avr/io.h>
#include <inttypes.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "harddef.h"
#include "makra.h"
#include "lcd.h"
#include "langsys.h"
#include "system.h"
#include "app.h"

void system_run(void)
{
	//Inicjacja wyprowadzeñ
	DDR(LCD_CTRLPORT) = (1<<LCD_E | 1<<LCD_RW | 1<<LCD_RS | 1<<LCD_LED);
	PORT(LCD_CTRLPORT) = ~(1<<LCD_E | 1<<LCD_LED);
	PORT(SW_PORT) = 1<<SW1 | 1<<SW2;
	//Inicjacja wyœwietlacza
	lcd_Init();
	lcd_SetStatus(LCD_STATUS_DISP);
	
	//Wyswietl zapytanie o jêzyk
	fputs_P(PSTR("S1 - "), lcd_GetFile());
	fputs_P(langsys_GetLangName(0), lcd_GetFile());
	lcd_GoTo(0, 1);
	fputs_P(PSTR("S2 - "), lcd_GetFile());
	fputs_P(langsys_GetLangName(1), lcd_GetFile());
	lcd_Update();
	
	//Oczekiwanie na przyciski
	langsys_Select(sw_wait()-1);
	
	//Wywo³anie aplikacji
	app_run();
}

