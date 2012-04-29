#ifndef LANGSYS_H_INCLUDED
#define LANGSYS_H_INCLUDED

//___________________________________________________________________________
//Funkcje interfejsu
inline uint8_t langsys_GetNumOfLangs(void);
inline void langsys_Select(uint8_t index);
inline uint8_t langsys_GetSelected(void);
prog_char* langsys_GetLangName(uint8_t index);
LCD_LOCAL_PGM* langsys_GetSpec(void);
prog_char* langsys_GetText(uint8_t index);

//__________________________________________________________________
//Indeksy dla poszczególnych napisów
enum
{
	IDS_LANGNAME,
	IDS_Start
};

#endif //LANGSYS_H_INCLUDED

