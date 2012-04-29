#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

//Zawartoœæ umieszczamy tutaj
#include <avr/pgmspace.h>
#include <stdio.h>

typedef struct LCD_LOCAL
{
	uint8_t matrix[8];
	char cAlt;
}LCD_LOCAL_PGM PROGMEM;

//Definicja funkcji interfejsu
void lcd_Init(void);
void lcd_Cls(void);
void lcd_SetStatus(uint8_t status);
	#define LCD_STATUS_BLINK 0x01
	#define LCD_STATUS_CURSOR 0x02
	#define LCD_STATUS_DISP 0x04
inline void lcd_UpdateStatus(void);
inline void lcd_GoToAdr(uint8_t adr);
	#define lcd_GoTo(x, y) \
		lcd_GoToAdr(y*LCD_SX+x);
void lcd_UpdateCurPos(void);
uint8_t lcd_PrepareSpec(void);
void lcd_UpdateCGRAM(void);
void lcd_UpdateText(void);
void lcd_Update(void);

inline FILE* lcd_GetFile(void);

#endif //LCD_H_INCLUDED
