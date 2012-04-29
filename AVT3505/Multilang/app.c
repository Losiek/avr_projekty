#include <avr/io.h>
#include <inttypes.h>
#include <stdio.h>

#include "harddef.h"
#include "makra.h"
#include "lcd.h"
#include "langsys.h"
#include "system.h"
#include "app.h"

void app_run(void)
{
	//Wyœwietlenie przywitania
	lcd_Cls();
	fputs(langsys_GetText(IDS_Start), lcd_GetFile());
	lcd_Update();
	
	for(;;)
	{
	}
}
