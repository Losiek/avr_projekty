/////////////////////////////////////////////////////////////////////////////////////////////
// lcd.c - plik �r�d�owy do obs�ugi alfanumerycznego wy�wietlacza LCD.
//			! Dynamiczne przydzielanie znak�w specjalnych !
//
// Autor: Rados�aw Koppel	Kompilator: WinAVR 20060125
/////////////////////////////////////////////////////////////////////////////////////////////
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <string.h>
#include <avr/pgmspace.h>

#include "makra.h"
#include "harddef.h"
#include "lcd.h"
#include "local.h"

//__________________________________________________________________________________________
// Definicje sta�ych
// Komendy steruj�ce wy�wietlaczem
#define LCDC_CLS		0x01
#define LCDC_HOME		0x02
#define LCDC_MODE		0x04
	#define LCDC_MODER		0x02
	#define LCDC_MODEL		0
	#define LCDC_MODEMOVE	0x01
#define LCDC_ON 		0x08
	#define LCDC_ONDISPLAY	0x04
	#define LCDC_ONCURSOR	0x02
	#define LCDC_ONBLINK	0x01
#define LCDC_SHIFT		0x10
	#define LCDC_SHIFTDISP	0x08
	#define LCDC_SHIFTR		0x04
	#define LCDC_SHIFTL		0
#define LCDC_FUNC		0x20
	#define LCDC_FUNC8b		0x10
	#define LCDC_FUNC4b		0
	#define LCDC_FUNC2L		0x08
	#define LCDC_FUNC1L		0
	#define LCDC_FUNC5x10	0x4
	#define LCDC_FUNC5x7	0
#define LCDC_CGA		0x40
#define LCDC_DDA		0x80

//__________________________________________________________________________________________
// Deklaracje funkcji kt�re musz� by� tutaj zdeklarowane
static int lcd_put(char c, FILE* f);

//__________________________________________________________________________________________
// Zmienne
// Korzystamy z nowej mo�liwo�ci avrlibc 1.4
static FILE lcd_fLCD_temp = FDEV_SETUP_STREAM(lcd_put, NULL, _FDEV_SETUP_WRITE);
#define lcd_fLCD (&lcd_fLCD_temp)

// Uwaga - wy�wietlacz niewykorzystuje kod�w 0x80-0x9f. To 32 kody kt�re zostan� wykorzystane dla znak�w specjalnych
static char lcd_buffer[LCD_SX*LCD_SY];
// Aktualne pozycja kursora (dotyczy bufora i jest aktualizwana przy od�wierzaniu napisu)
static uint8_t lcd_curpos;
// Informacja o tym jakie znaki specjalne zosta�y zapisane do wy�wietlacza
static uint8_t lcd_spec[8];
// Zmienna przechowuj�ca flagi na temat w��czenia wy�wietlacza (wy�wietlacz, kursor, migaj�cy kursor)
static uint8_t lcd_status;

//__________________________________________________________________________________________
// Obs�uga LCD - funkcje niskiego poziomu

// Impuls ENABLE - przede wszystkim dla zapisu
// Odczyt wymaga troch� bardziej skomplikowanej sekwencji
#define lcd_epulse() \
	{PORT(LCD_CTRLPORT) |= 1<<LCD_E; \
	_delay_us(0.25); \
	PORT(LCD_CTRLPORT) &= ~(1<<LCD_E);}

// Odbi�r danej - funkcja wewn�trzna
static inline uint8_t lcd_Get(void)
{
	uint8_t dana; 
	// Ustawienie portu lcd jako wej�cia
	DDR(LCD_DATAPORT) = 0x00; 
	// Aktywacja odczytu
	PORT(LCD_CTRLPORT) |= 1<<LCD_E; 
	// \/ Uwaga - sprawdzi�em eksperymentalnie, 
	// \/ �e 0.5us to czasami za ma�o
	// \/ przy pod��czonym programatorze wy�wietlacz
	// \/ wci�� gubi� dane
	_delay_us(1);
	// Skopiowanie danych z wyj�cia modu�u
	dana = PIN(LCD_DATAPORT); 
	// Deaktywacja wyj�cia ENABLE
	PORT(LCD_CTRLPORT) &= ~(1<<LCD_E); 
	return dana; 
}

uint8_t lcd_GetBF(void)
{
	// Wysterowanie wyprowadze� do odczytu statusu
	PORT(LCD_CTRLPORT) |= 1<<LCD_RW; 
	PORT(LCD_CTRLPORT) &= ~(1<<LCD_RS);
	// Odczekanie wymaganego przez sterownik czasu
	_delay_us(0.25);
	// Odczyt danej
	return lcd_Get(); 
}

// Sprawdza flag� zaj�to�ci i czeka na jej wyzerowanie
void lcd_WaitBF(void)
{
	// Bit zaj�to�ci to bit najstarszy
	while(0 != (0x80 & lcd_GetBF())) {}; 
}

// Wys�anie danej - funkcja wewn�trzna
static void lcd_Send(uint8_t dana)
{
	// Ustawienie portu lcd jako wyj�cia
	DDR(LCD_DATAPORT) = 0xFF; 
	// Pres�anie na port danej
	PORT(LCD_DATAPORT) = dana; 
	// Przes�anie do lcd
	lcd_epulse(); 
}

// Wys�anie danej do pami�ci DDRAM lub CGRAM
// Zale�nie od ustawionego adresu
void lcd_SendData(uint8_t dana)
{
	lcd_WaitBF();
	// Wysterowanie wyprowadze� dla zapisu danej
	PORT(LCD_CTRLPORT) &= ~(1<<LCD_RW); 
	PORT(LCD_CTRLPORT) |= 1<<LCD_RS; 
	// Odczekanie oraz wys�anie
	_delay_us(0.25); 
	lcd_Send(dana); 
}

// Wys�anie instrukcji steruj�cej
void lcd_SendInstr(uint8_t dana)
{
	lcd_WaitBF();
	// Wysterowanie wyprowadze� dla zapisu instrukcji
	PORT(LCD_CTRLPORT) &= ~(1<<LCD_RW | 1<<LCD_RS);
	// Odczekanie oraz wys�anie
	_delay_us(0.25); 
	lcd_Send(dana); 
}

//__________________________________________________________________________________________
// Funkcje obs�uguj�ce wej�cie wyj�cie strumienia
static int lcd_put(char c, FILE* f)
{
	// Zabezpieczenie przed przepe�nieniem
	if(lcd_curpos >= ELEMS(lcd_buffer))
		lcd_curpos = 0;
	// Zapis do bufora
	lcd_buffer[lcd_curpos++] = c;
	return 0;
}

//__________________________________________________________________________________________
// Funkcje interfejsu

// inicjacja LCD (uwaga - nie w��czam wy�wietlacza ani kursora)
void lcd_Init(void)
{
	lcd_Cls();
	PORT(LCD_CTRLPORT) &= ~(1<<LCD_RW | 1<<LCD_RS); 
	_delay_us(0.25); 
	lcd_Send(LCDC_FUNC | LCDC_FUNC8b); 
	_delay_ms(4.3); 
	lcd_Send(LCDC_FUNC | LCDC_FUNC8b); 
	_delay_us(200); 
	lcd_Send(LCDC_FUNC | LCDC_FUNC8b); 
	// Ju� mo�na sprawdza� BF
#if LCD_SY == 1
	lcd_SendInstr(LCDC_FUNC | LCDC_FUNC8b | LCDC_FUNC1L); 
#else
	lcd_SendInstr(LCDC_FUNC | LCDC_FUNC8b | LCDC_FUNC2L); 
#endif
	lcd_SendInstr(LCDC_ON);
	lcd_SendInstr(LCDC_CLS);
	lcd_SendInstr(LCDC_MODE | LCDC_MODER); 
	lcd_WaitBF();
	// Koniec inicjacji. Wyj�tkowo czekam na zako�czenie. Normalnie BF jest sprawdzana przed wykonaniem indtrukcji 
	// - daje to wi�ksz� oszcz�dno�� czasu. Podczas inicjacji nie ma to wielkiego znaczenia. 
}

// Zerowanie bufora wy�wietlacza
void lcd_Cls(void)
{
	lcd_curpos = 0;
	memset(lcd_buffer, ' ', sizeof(lcd_buffer));
}

// kontrola w��czenia wy�wietlacza, kursora, migaj�cego kursora
void lcd_SetStatus(uint8_t status)
{
	lcd_status = status;
}

// Odswierzenie statusu
inline void lcd_UpdateStatus(void)
{
	lcd_SendInstr(LCDC_ON | lcd_status);
}

// Funkcja wewn�trzna podaj�ca pocz�tek danej linii
static uint8_t lcd_LineStart(uint8_t line)
{
#if LCD_SY == 1
	return LCD_LINE1;
#elif LCD_SY == 2
	if(line == 0)
		return LCD_LINE1;
	else
		return LCD_LINE2;
#elif LCD_SY == 4
	if(line == 0)
		return LCD_LINE1;
	else if(linenum == 1)
		return LCD_LINE2;
	else if(linenum == 2)
		return LCD_LINE3;
	else
		return LCD_LINE4;
#else
#error Unknown display type!
#endif
}

// Ustawienie kursora
inline void lcd_GoToAdr(uint8_t adr)
{
	lcd_curpos = adr;
}

// Odswierzenie pozycji kursora
void lcd_UpdateCurPos(void)
{
	// Obliczenie adresu w wy�wietlaczu
	uint8_t adres = 
		lcd_LineStart(lcd_curpos / LCD_SX) + (lcd_curpos % LCD_SX);
	// Wys�anie instrkucji ustawiaj�cej kursor
	lcd_SendInstr(LCDC_DDA | adres);
}

// Wewn�trzna pomocnicza funkcja sprawdzaj�ca czy dany znak jest znakiem specjalnym
static uint8_t lcd_IsSpec(char c)
{
	// Przydzia� znak�w specjalnych na obszar kt�rego LCD nie wykorzystuje
	return (c >= 0x80) && (c <= 0x9f);
}

// Zamiana znaku specjalnego na odpowiedni numer w tablicy symboli
static inline uint8_t lcd_Spec2Index(char c)
{
	return c-0x80;
}

// Funkcja sprawdzajaca czy w tablicy jest podany znak
// Zwraca: indeks w tablicy gdzie znajduje sie dany znak
//  lub kod znaku alternatywnego (zawsze >= 0x20)
static uint8_t lcd_GetSpec(uint8_t s_index)
{
	uint8_t a; 
	for(a=0; a<ELEMS(lcd_spec); a++)
	{
		// 0xff oznacza, �e nie ma ju� dalej wpis�w
		if(lcd_spec[a] == 0xff)
			break; 
		// Je�li znaleziono...
		else if(lcd_spec[a] == s_index)
			return a; 
	}
	// Nic nie znaleziono
	return pgm_read_byte(&(local_lcdspec[s_index].cAlt));
}

// Makro pomocnicze
#define LCD_SPECNF(spec) (spec > 7)

// Funkcja przydzielaj�ca znaki specjalne
// Zwraca ilo�� r�nych znak�w jak� znaleziono. Je�li > 8 cz�ci znak�w nie uda�o si� przydzieli�
uint8_t lcd_PrepareSpec(void)
{
	// Analizuj� ca�y �a�cuch danych i zapisuj� kolejno znalezione znaki
	char* pbuffer = lcd_buffer;
	uint8_t n;
	uint8_t cnt=0;
	char znak;
	// Na pocz�tku "zerowanie" tablicy znak�w specjalnych
	memset(lcd_spec, 0xff, sizeof(lcd_spec));
	// dodawanie wpis�w
	for(n=0; n<ELEMS(lcd_buffer); n++)
	{
		znak = *pbuffer++;
		// Je�li znaleziony znak jest specjalny
		if(lcd_IsSpec(znak))
		{
			// Przeszukanie tablicy
			znak = lcd_Spec2Index(znak);
			// Je�li nie znaleziono - dodawanie
			if(LCD_SPECNF(lcd_GetSpec(znak)))
			{
				// dodaj do tablicy tylko je�li 
				// nie wyst�pi�o przepe�nienie
				if(cnt < ELEMS(lcd_spec))
					lcd_spec[cnt] = znak;
				// Licznik zwi�kszany zawsze
				// dla statystyki
				++cnt;
			}
		}
	}
	return cnt;
}

// Funkcja zapisuj�ca definicje znak�w do pami�ci CGRAM wy�wietlacza
void lcd_UpdateCGRAM(void)
{
	// Ustawiam adres w module na pocz�tek definicji znak�w: 
	lcd_SendInstr(LCDC_CGA); 
	// Przesy�anie blok�w - tylko do czasu a� mam 0xff w tablicy - koniec danych
	uint8_t a; 
	for(a=0; a<ELEMS(lcd_spec); a++)
	{
		// 0xff oznacza koniec danych
		if(lcd_spec[a] == 0xff) break;
		// Wska�nik na pocz�tek danych wygl�du znaku
		uint8_t* pdata = local_lcdspec[lcd_spec[a]].matrix;
		// Wysy�am kolejny indeks
		uint8_t bait;
		for(bait=0; bait<8; bait++)
		{
			lcd_SendData(pgm_read_byte(pdata++)); 
		}
	}
}

// Funckcja od�wierzaj�ca dane w pami�ci DDRAM wy�wietlacza
// Aby wy�wietlanie przebieg�o prawid�owo pami�� CGRAM musi by� ju� ustawiona
void lcd_UpdateText(void)
{
	uint8_t x, y;
	
	char *pbuffer = lcd_buffer;
	// Przetwarzanie ka�dej linii oddzielnie
	for(y=0; y<LCD_SY; y++)
	{
		lcd_SendInstr(LCDC_DDA | lcd_LineStart(y));
		// Wys�anie wszystkich znak�w z danej linii
		for(x=0; x<LCD_SX; x++)
		{
			uint8_t znak = *pbuffer++;
			if(lcd_IsSpec(znak))
				znak = lcd_GetSpec(lcd_Spec2Index(znak));
			// Znak gotowy do wys�ania
			lcd_SendData(znak);
		}
	}
}

// Od�wierzenie wszystkich danych wy�wietlacza
void lcd_Update(void)
{
	lcd_PrepareSpec();
	lcd_UpdateCGRAM();
	// Wy��czenie kursora na czas od�wierzania
	lcd_SendInstr(LCDC_ON | (lcd_status & LCD_STATUS_DISP));
	lcd_UpdateText();
	lcd_UpdateCurPos();
	// Ponowne w��czenie kursora (je�li by� w��czony)
	lcd_UpdateStatus();
}

//__________________________________________________________________________________________
// Funkcja podaj�ca uchwyt do strumienia
inline FILE* lcd_GetFile(void)
{
	return lcd_fLCD;
}
