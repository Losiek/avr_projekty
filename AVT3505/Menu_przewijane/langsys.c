#include <inttypes.h>
#include <avr/pgmspace.h>

#include "makra.h"
#include "lcd.h"
#include "langsys.h"
//Nag��wki j�zyk�w
#include "lang/english.h"
#include "lang/polski.h"

static uint8_t langsys_sel = 0; //Wybrany j�zyk

//Definicja informacji o znakach specjalnych
const LCD_LOCAL_PGM* langsys_lcdspec[] PROGMEM =
{
	NULL,
	PL_lcdspec
};

//Tablica wszystkich posiadanych napis�w
//                   [j�zyk][tekst]
const prog_char* langsys_strTable[2][2] PROGMEM =
{
	{
		EN_strDisplay,
		EN_strStart
	},
	{
		PL_strDisplay,
		PL_strStart
	}
};

//Ilo�� "zainstalowanych" j�zyk�w
inline uint8_t langsys_GetNumOfLangs(void)
{
	return ELEMS(langsys_strTable);
}

//Wyb�r j�zyka
inline void langsys_Select(uint8_t index)
{
	langsys_sel =index;
}

//Pobieranie informacji o wybranym j�zyku
inline uint8_t langsys_GetSelected(void)
{
	return langsys_sel;
}

//Pobieranie nazwy j�zyka o podanym indeksie
prog_char* langsys_GetLangName(uint8_t index)
{
	if(index > ELEMS(langsys_strTable))
		return NULL;
	return (prog_char*)pgm_read_word_near(&langsys_strTable
		[index][IDS_LANGNAME]);
}

//Pobieranie wska�nika nainformacj� o znakach specjalnych
LCD_LOCAL_PGM* langsys_GetSpec(void)
{
	return (LCD_LOCAL_PGM*)pgm_read_word_near(&langsys_lcdspec[langsys_sel]);
}

//Pobieranie wybranego napisu
prog_char* langsys_GetText(uint8_t index)
{
	return (prog_char*)pgm_read_word_near(&langsys_strTable[langsys_sel][index]);
}

