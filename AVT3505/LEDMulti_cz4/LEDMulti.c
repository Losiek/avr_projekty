///////////////////////////////////////////////////////////
// LEDMulti2.c - obs³uga wyœwietlacza multipleksowanego LED za pomoc¹ przerwañ
//Kurs C czêœæ 2
///////////////////////////////////////////////////////////
#include <avr/io.h>
#include <inttypes.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

// Definicje wyprowadzeñ
#define LED_A 4
#define LED_B 3
#define LED_C 5
#define LED_D 2
#define LED_E 6
#define LED_F 0
#define LED_G 7
#define LED_DP 1
#define LEDPORT PORTB
#define LEDDDR DDRB
#define COM1 5
#define COM2 4
#define COM3 3
#define COM4 2
#define COMPORT PORTD
#define COMDDR DDRD
#define SWPORT PORTE
#define SWDDR DDRE
#define SWPIN PINE
#define SW1 0
#define SW2 2

volatile uint8_t g_bDelay; // 122Hz
uint8_t g_DaneWyswietlacza[4];
uint8_t g_DaneCom[] PROGMEM = {~(1<<COM1), ~(1<<COM2), ~(1<<COM3), ~(1<<COM4)};

//Tablica wzrów cyfr w kodzie szesnastkowym.
prog_uint8_t g_WzoryCyfr[16] = {
		(uint8_t)~(1<<LED_A | 1<<LED_B | 1<<LED_C | 1<<LED_D | 1<<LED_E | 1<<LED_F), // 0	
		(uint8_t)~(1<<LED_B | 1<<LED_C), // 1
		(uint8_t)~(1<<LED_A | 1<<LED_B | 1<<LED_G | 1<<LED_E | 1<<LED_D), // 2
		(uint8_t)~(1<<LED_A | 1<<LED_B | 1<<LED_C | 1<<LED_D | 1<<LED_G), // 3
		(uint8_t)~(1<<LED_B | 1<<LED_C | 1<<LED_G | 1<<LED_F), // 4
		(uint8_t)~(1<<LED_A | 1<<LED_F | 1<<LED_G | 1<<LED_C | 1<<LED_D), // 5
		(uint8_t)~(1<<LED_A | 1<<LED_F | 1<<LED_E | 1<<LED_D | 1<<LED_C |1<< LED_G), // 6
		(uint8_t)~(1<<LED_A | 1<<LED_B | 1<<LED_C), // 7
		(uint8_t)~(1<<LED_A | 1<<LED_B | 1<<LED_C | 1<<LED_D | 1<<LED_E | 1<<LED_F | 1<<LED_G), // 8
		(uint8_t)~(1<<LED_G | 1<<LED_F | 1<<LED_A | 1<<LED_B | 1<<LED_C | 1<<LED_D), // 9
		(uint8_t)~(1<<LED_A | 1<<LED_B | 1<<LED_C | 1<<LED_E | 1<<LED_F | 1<<LED_G), // A
		(uint8_t)~(1<<LED_C | 1<<LED_D | 1<<LED_E | 1<<LED_F | 1<<LED_G), // B
		(uint8_t)~(1<<LED_A | 1<<LED_D | 1<<LED_E | 1<<LED_F), // C
		(uint8_t)~(1<<LED_B | 1<<LED_C | 1<<LED_D | 1<<LED_E | 1<<LED_G), // D
		(uint8_t)~(1<<LED_A | 1<<LED_D | 1<<LED_E | 1<<LED_F | 1<<LED_G), // E
		(uint8_t)~(1<<LED_A | 1<<LED_E | 1<<LED_F | 1<<LED_G) // F
	};

//Zmienna przechowuj¹ca wartoœæ licznika.
uint16_t g_Licznik = 0;

//Funkcja wyœwitla w kodzie dziesiêtnym podan¹ liczbê.
void WyswietlDEC(uint16_t var) {
	uint8_t n;
	if(var > 9999)
	{
		for(n=0; n<4; n++)
		{
			g_DaneWyswietlacza[n] = (uint8_t)~(1<<LED_G);
		}
		return;
	}
	
	for(n=4; n>0; n--)
	{
		g_DaneWyswietlacza[n-1] = pgm_read_byte(&g_WzoryCyfr[var%10]);
		var /= 10;
	}	
}	

void ObslozPrzycisk(uint8_t maska, void(*proc)(void))
{
	if(!(SWPIN & maska))
	{
		//usuwanie drgañ styków.
		g_bDelay = 6; //50ms
		while(g_bDelay !=0) {}
		if(!(SWPIN & maska))
		{
			proc();
			g_bDelay = 120; //1s
			do
			{
				if(g_bDelay==0)
				{
					g_bDelay = 6; //50ms
					proc();
				}
			}while(!(SWPIN & maska));
		}
	}
}

void Plus(void)
{
	++g_Licznik;
	WyswietlDEC(g_Licznik);
}

void Minus(void)
{
	--g_Licznik;
	WyswietlDEC(g_Licznik);
}

int main(void)
{
	//Dzielenie zegara
	CLKPR = 1<<CLKPCE;
	CLKPR = 1<<CLKPS0;
	/////////////////////////////
	// inicjacja
	LEDDDR = 0xff; 
	COMDDR = 1<<COM1 | 1<<COM2 | 1<<COM3 | 1<<COM4;
	//w¹czenie podci¹gania wyjœæ
	SWPORT = 1<<SW1 | 1<<SW2;
	//Timer0.
	TCCR0 = 1<<CS01 | 1<<CS00;
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
		ObslozPrzycisk(1<<SW1, Plus); 
		ObslozPrzycisk(1<<SW2, Minus);
	
	}

	return 0; 
}
//____________________________________________________________________________________________
//Obs³uga przerwañ
SIGNAL(SIG_OVERFLOW0)
{
		static uint8_t AktWyswietlacz = 0;
		//Wpisanie do licznika pocz¹tkowej wartoœci.
		TCNT0 = 128;
		//Wygaszenie wyœwietlaczy.
		COMPORT |= 1<<COM1 | 1<<COM2 | 1<<COM3 | 1<<COM4;
		//Wys³anie odpowiedniej danej.
		LEDPORT = g_DaneWyswietlacza[AktWyswietlacz];
		//W³¹czenie odpowiedniego wyœwietlacza.
		//Odwo³anie do pamiêci programu.
		COMPORT &= pgm_read_byte(&g_DaneCom[AktWyswietlacz]);
		
		//Zwiêkszanie stanu zmiennej wskazuj¹cej wyœwietlacz
		++AktWyswietlacz;
		if (AktWyswietlacz > 3) {
			//Zmienna zapobiegaj¹ca niepotrzebnemu kopiowaniu
			//zmiennej g_bDelay (typ volatile!)
			uint8_t delay;
			AktWyswietlacz = 0;
			//Obs³uga opóŸnieñ
			delay = g_bDelay;
			if(delay > 0)
			{
				--delay;
				g_bDelay = delay;
			}
		}
}
	
