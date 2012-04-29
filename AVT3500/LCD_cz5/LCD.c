#include <avr/io.h>
#include <inttypes.h>
#include <avr/pgmspace.h>

//Makra upraszczaj¹ce dostêp do portów.
//*** Port
#define PORT(x) XPORT(x)
#define XPORT(x) (PORT##x)
//*** Pin
#define PIN(x) XPIN(x)
#define XPIN(x) (PIN##x)
//*** DDR
#define DDR(x) XDDR(x)
#define XDDR(x) (DDR##x)

//Definicja wyprowadzeñ.
#define LED_PORT D
#define SW_PORT D
#define SW1 0
#define SW2 1

// Definicje wyprowadzeñ
#define LCD_RS 2
#define LCD_RSPORT B
#define LCD_E 1
#define LCD_EPORT B
// Port danych: zak³adam, ¿e wyprowadzenia s¹ po³¹czone kolejno. 
// Definiowane jest jedynie przesuniêcie oznaczaj¹ce na którym wyprowadzeniu jest D4
#define LCD_DPORT B
#define LCD_D4 4 // D5, D6, D7 - po³¹czone kolejno
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7

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

//________________________________________
//Generowanie opóŸnieñ
#define delay250ns() {asm volatile("nop"::);}

#define delayus8(t)\
	{asm volatile( \
		"delayus8_loop%=: \n\t"\
			"nop \n\t"\
			"dec %[ticks] \n\t"\
			"brne delayus8_loop%= \n\t"\
	: :[ticks]"r"(t) );}
	// DEC - 1 cykl, BRNE 2 cykle, + 1xnop. Zegar 4MHz

void delay100us8(uint8_t t)
{
	while(t>0)
	{
		delayus8(100); 
		--t; 
	}
}
//Koniec opóŸnieñ
//_________________________________________

#define LCD_EPULSE() \
	{PORT(LCD_EPORT) |= 1<<LCD_E; \
	delay250ns(); \
	PORT(LCD_EPORT) &= ~(1<<LCD_E);}

void LCDsendHalf(uint8_t data)
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

void LCDsend(uint8_t data)
{
	// Starsza czêœæ
	LCDsendHalf(data>>4);
	// M³odsza czêœæ
	LCDsendHalf(data);
	delayus8(120);
}

// Funkcje interfejsu
void LCDcommand(uint8_t command)
{
	PORT(LCD_RSPORT) &= ~(1<<LCD_RS);
	LCDsend(command);
}

void LCDdata(uint8_t data)
{
	PORT(LCD_RSPORT) |= 1<<LCD_RS;
	LCDsend(data);
}

void LCDcls(void)
{
	LCDcommand(LCDC_CLS);
	delay100us8(48);
}

void LCDhome(void)
{
	LCDcommand(LCDC_HOME);
	delay100us8(48);
}

void LCDinit(void)
{
	delay100us8(150);
	PORT(LCD_RSPORT) &= ~(1<<LCD_RS);
	LCDsendHalf(LCDC_FUNC|LCDC_FUNC8b); 
	delay100us8(41);
	LCDsendHalf(LCDC_FUNC|LCDC_FUNC8b);
	delay100us8(2);
	LCDsendHalf(LCDC_FUNC|LCDC_FUNC4b);
	delay100us8(2);
	// Teraz jest ju¿ 4b. Koniec korzystania z sendHalf
	LCDcommand(LCDC_FUNC|LCDC_FUNC4b|LCDC_FUNC2L|LCDC_FUNC5x7);
	LCDcommand(LCDC_ON);
	LCDcls();
	LCDcommand(LCDC_MODE|LCDC_MODER);
	LCDcommand(LCDC_ON|LCDC_ONDISPLAY|LCDC_ONCURSOR);
}

// Koniec obs³ugi wyœwietlacza
//--------------------------------------------------
void LCDstr_P(prog_char* str)
{
	char znak;
	while(0 != (znak = pgm_read_byte(str++)))
	LCDdata(znak);
}

// START
int main(void)
{
	//----------------------------------------------
	// Inicjacja - uwaga: uproszczona, mo¿e wymagaæ 
	// zmian przy zmianie przyporz¹dkowania portów
	DDR(LCD_DPORT) = 1<<LCD_E | 1<<LCD_RS | 
		0x0F<<LCD_D4;
	
	LCDinit();
	// Koniec inicjacji
	//---------------------------------------------- 
	LCDcls(); 
	
	LCDstr_P((prog_char*)PSTR("Witaj!"));
	LCDcommand(LCDC_DDA|64);
	LCDstr_P((prog_char*)PSTR("Lancuch z FLASHa"));
	
	
	
	return 0;
}
