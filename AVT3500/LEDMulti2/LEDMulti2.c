///////////////////////////////////////////////////////////
// LEDMulti2.c - obs³uga wyœwietlacza multipleksowanego LED za pomoc¹ przerwañ
//Kurs C czêœæ 2
///////////////////////////////////////////////////////////
#include <avr/io.h>
#include <inttypes.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/delay.h>

// Definicje wyprowadzeñ
#define LED_A 0
#define LED_B 1
#define LED_C 2
#define LED_D 3
#define LED_E 4
#define LED_F 5
#define LED_G 6
#define LED_DP 7
#define LEDPORT PORTB
#define LEDDDR DDRB
#define COM1 6
#define COM2 5
#define COM3 4
#define COM4 3
#define COMPORT PORTD
#define COMDDR DDRD

uint8_t g_AktWyswietlacz = 0;
uint8_t g_DaneWyswietlacza[4];
uint8_t g_DaneCom[] PROGMEM = {~(1<<COM1), ~(1<<COM2), ~(1<<COM3), ~(1<<COM4)};

//Tablica wzrów cyfr w kodzie szesnastkowym.
prog_uint8_t g_WzoryCyfr[16] = {
		~(1<<LED_A | 1<<LED_B | 1<<LED_C | 1<<LED_D | 1<<LED_E | 1<<LED_F), // 0	
		~(1<<LED_B | 1<<LED_C), // 1
		~(1<<LED_A | 1<<LED_B | 1<<LED_G | 1<<LED_E | 1<<LED_D), // 2
		~(1<<LED_A | 1<<LED_B | 1<<LED_C | 1<<LED_D | 1<<LED_G), // 3
		~(1<<LED_B | 1<<LED_C | 1<<LED_G | 1<<LED_F), // 4
		~(1<<LED_A | 1<<LED_F | 1<<LED_G | 1<<LED_C | 1<<LED_D), // 5
		~(1<<LED_A | 1<<LED_F | 1<<LED_E | 1<<LED_D | 1<<LED_C |1<< LED_G), // 6
		~(1<<LED_A | 1<<LED_B | 1<<LED_C), // 7
		~(1<<LED_A | 1<<LED_B | 1<<LED_C | 1<<LED_D | 1<<LED_E | 1<<LED_F | 1<<LED_G), // 8
		~(1<<LED_G | 1<<LED_F | 1<<LED_A | 1<<LED_B | 1<<LED_C | 1<<LED_D), // 9
		~(1<<LED_A | 1<<LED_B | 1<<LED_C | 1<<LED_E | 1<<LED_F | 1<<LED_G), // A
		~(1<<LED_C | 1<<LED_D | 1<<LED_E | 1<<LED_F | 1<<LED_G), // B
		~(1<<LED_A | 1<<LED_D | 1<<LED_E | 1<<LED_F), // C
		~(1<<LED_B | 1<<LED_C | 1<<LED_D | 1<<LED_E | 1<<LED_G), // D
		~(1<<LED_A | 1<<LED_D | 1<<LED_E | 1<<LED_F | 1<<LED_G), // E
		~(1<<LED_A | 1<<LED_E | 1<<LED_F | 1<<LED_G) // F
	};

//Zmienna przechowuj¹ca wartoœæ licznika.
uint16_t g_Licznik = 0;

//Funkcja wyœwitla w kodzie szesnastkowym podan¹ liczbê.
void WyswietlHex(uint16_t var) {
	g_DaneWyswietlacza[0] = pgm_read_byte(&g_WzoryCyfr[var>>(3*4)]);
	g_DaneWyswietlacza[1] = pgm_read_byte(&g_WzoryCyfr[(var>>(2*4))&0xf]);
	g_DaneWyswietlacza[2] = pgm_read_byte(&g_WzoryCyfr[(var>>(1*4))&0xf]);
	g_DaneWyswietlacza[3] = pgm_read_byte(&g_WzoryCyfr[var&0xf]);
}	


int main(void)
{
	/////////////////////////////
	// inicjacja
	LEDDDR = 0xff; 
	COMDDR = 1<<COM1 | 1<<COM2 | 1<<COM3 | 1<<COM4;
	//Timer0.
	TCCR0 = 1<<CS01; //| 1<<CS00;
	TIMSK = 1<<TOIE0;
	//Globalne zezwolenie przerwañ.
	sei();
	// koniec inicjacji.
	/////////////////////////////
	
	/*Wpisanie próbnych wartoœci.
	g_DaneWyswietlacza[0] = ~(1<<LED_B | 1<<LED_C | 1<<LED_A | 1<<LED_E | 1<<LED_F | 1<<LED_D);
	g_DaneWyswietlacza[2] = ~(1<<LED_D | 1<<LED_A | 1<<LED_G | 1<<LED_E | 1<<LED_F);
	g_DaneWyswietlacza[1] = ~(1<<LED_F | 1<<LED_E | 1<<LED_D);
	g_DaneWyswietlacza[3] = ~(1<<LED_B | 1<<LED_C | 1<<LED_G| 1<<LED_E | 1<<LED_F);
	*/
	
	for(;;)
	{
		WyswietlHex(g_Licznik);
		_delay_loop_2(0xffff);
		g_Licznik++;
	}

	return 0; 
}
//____________________________________________________________________________________________
//Obs³uga przerwañ
SIGNAL(SIG_OVERFLOW0)
{
	//Wygaszenie wyœwietlaczy.
		COMPORT |= 1<<COM1 | 1<<COM2 | 1<<COM3 | 1<<COM4;
		//Wys³anie odpowiedniej danej.
		LEDPORT = g_DaneWyswietlacza[g_AktWyswietlacz];
		//W³¹czenie odpowiedniego wyœwietlacza.
		//Odwo³anie do pamiêci programu.
		COMPORT &= pgm_read_byte(&g_DaneCom[g_AktWyswietlacz]);
		
		//Zwiêkszanie stanu zmiennej wskazuj¹cej wyœwietlacz
		++g_AktWyswietlacz;
		if (g_AktWyswietlacz > 3) {
			g_AktWyswietlacz =0;
			}
}