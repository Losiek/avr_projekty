#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

// Komendy steruj¹ce wyœwietlaczem
#define LCDC_CLS			0x01
#define LCDC_HOME			0x02
#define LCDC_MODE			0x04
	#define LCDC_MODER		0x02
	#define LCDC_MODEL		0
	#define LCDC_MODEMOVE	0x01
#define LCDC_ON 			0x08
	#define LCDC_ONDISPLAY	0x04
	#define LCDC_ONCURSOR	0x02
	#define LCDC_ONBLINK	0x01
#define LCDC_SHIFT			0x10
	#define LCDC_SHIFTDISP	0x08
	#define LCDC_SHIFTR		0x04
	#define LCDC_SHIFTL		0
#define LCDC_FUNC			0x20
	#define LCDC_FUNC8b		0x10
	#define LCDC_FUNC4b		0
	#define LCDC_FUNC2L		0x08
	#define LCDC_FUNC1L		0
	#define LCDC_FUNC5x10	0x4
	#define LCDC_FUNC5x7	0
#define LCDC_CGA			0x40
#define LCDC_DDA			0x80

//Deklaracja funkcji
void lcd_command(uint8_t command);
void lcd_data(uint8_t data);
void lcd_cls(void);
void lcd_home(void);
void lcd_init(void);

void lcd_str_P(prog_char* str);
void lcd_str(char* str);
void lcd_dec(int val);

#endif //LCD_H_INCLUDED
