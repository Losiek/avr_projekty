///////////////////////////////////////////////////////////
// LEDMulti.c - obs�uga wy�wietlacza multipleksowanego LED
//Kurs C cz�� 2
///////////////////////////////////////////////////////////
#include <avr/io.h>
#include <inttypes.h>

// Definicje wyprowadze�
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
uint8_t g_DaneCom[] = {~(1<<COM1), ~(1<<COM2), ~(1<<COM3), ~(1<<COM4)};


int main(void)
{
	/////////////////////////////
	// inicjacja
	LEDDDR = 0xff; 
	COMDDR = 1<<COM1 | 1<<COM2 | 1<<COM3 | 1<<COM4; 
	// koniec inicjacji
	/////////////////////////////
	
	//Wpisanie pr�bnych warto�ci.
	g_DaneWyswietlacza[0] = ~(1<<LED_B | 1<<LED_C | 1<<LED_A | 1<<LED_E | 1<<LED_F | 1<<LED_D);
	g_DaneWyswietlacza[2] = ~(1<<LED_D | 1<<LED_A | 1<<LED_G | 1<<LED_E | 1<<LED_F);
	g_DaneWyswietlacza[1] = ~(1<<LED_F | 1<<LED_E | 1<<LED_D);
	g_DaneWyswietlacza[3] = ~(1<<LED_B | 1<<LED_C | 1<<LED_G| 1<<LED_E | 1<<LED_F);
	
	for(;;)
	{
		//Wygaszenie wy�wietlaczy.
		COMPORT |= 1<<COM1 | 1<<COM2 | 1<<COM3 | 1<<COM4;
		//Wys�anie odpowiedniej danej.
		LEDPORT = g_DaneWyswietlacza[g_AktWyswietlacz];
		//W��czenie odpowiedniego wy�wietlacza.
		COMPORT &= g_DaneCom[g_AktWyswietlacz];
		
		//Zwi�kszanie stanu zmiennej wskazuj�cej wy�wietlacz
		++g_AktWyswietlacz;
		if (g_AktWyswietlacz > 3) {
			g_AktWyswietlacz =0;
			}
	}

	return 0; 
}
