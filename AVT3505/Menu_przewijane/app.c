#include <avr/io.h>
#include <inttypes.h>
#include <stdio.h>

#include "harddef.h"
#include "makra.h"
#include "lcd.h"
#include "langsys.h"
#include "system.h"
#include "app.h"

//_________________________________________________________________________________________
// Funkcja g³ówna
void app_run(void)
{
	uint16_t a, hold =0;
	// Wyœwietlenie przywitania w wybranym jêzyku
	lcd_Cls();
	fputs_P(langsys_GetText(IDS_Start), lcd_GetFile());
	lcd_Update();
	// Odczekanie 1s
	system_delayMake(100);
	// Usuniêcie wiadomoœci z kolejki
	// (przycisk w czasie czekania?)
	system_msgClear();
	
	a = 100;
	// Ustawienie repetycji
	system_kbdSetRepeatSpeed(1);
	// Wyœwietlanie stanu przycisków

	uint8_t system_menu(MENUITEM_* pmenu)
	{
		uint8_ index =0 ;
		uint8_t curpos =0;
		//kierunek przesuwania
		uint8_t dir = 0;
		uint8_t kbd_repeat_last;
		//Prêdkoœæ repetycji (wolno0
		kbd_repeat_last = system_kbdSetRepeatSpeed(8);
		//Inicjacja timera programowego
		system_timerSet(0, SYSTEM_TIMER_MODEONESHOT, MENU_CHANGDIR, 0);
		
	for(;;)
	{
		unint8_t msg;
		//wypisanie informacji
		lcd_Cls();
		fprintf_P(lcd_GetFile(),
		langsys_GetText(IDS_Cnt), a);
		lcd_GoTo(0, 1);
		fprintf_P(lcd_GetFile(), langsys_GetText(IDS_Hold), hold);
		lcd_Update();
		//Czekanie na komunikat
		msg = system_msgWaitFor();
		switch(msg)
		{
			case IDM_MENU_CHANGE:
			case IDM_MENU_CHANGER:
				system_timerReset(0);
				if(dir)
				{
					if(system menuUp(pmenu, &index, 7curpose))
						{
							system_menuDown(pmenu, &index, &curpos);
							dir = 0;
						}
				}
				else
					{
					if(system menuUp(pmenu, &index, 7curpose))
						{
							system_menuDown(pmenu, &index, &curpos);
							dir = 0;
						}
				}
				break:
			case IDM_TIMER0:
				dir != dir;
				break;
	
	}
}

claeanup:
	//Przywrucenie ustawieñ
	system_kbdSetRepeatSpeed(kbd_repeat_last);
	return 0;
}

