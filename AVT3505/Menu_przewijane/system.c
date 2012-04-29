#include <avr/io.h>
#include <inttypes.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "harddef.h"
#include "makra.h"
#include "lcd.h"
#include "langsys.h"
#include "system.h"
#include "app.h"

//Zmienna do generowania opóŸnieñ, zmniejczana do 0 co 10ms
static volatile uint8_t system_delay;
static struct SYSTEM_TIMER
{
	uint8_t mode;
	uint8_t start;
	uint8_t cnt;
}system_timer[3];

void system_run(void)
{	
	// Inicjacja wyprowadzeñ
	DDR(LCD_CTRLPORT) = (1<<LCD_E | 1<<LCD_RW | 1<<LCD_RS | 1<<LCD_LED); 
	PORT(LCD_CTRLPORT) = ~(1<<LCD_E | 1<<LCD_LED); 
	PORT(SW_PORT) = 1<<SW1 | 1<<SW2;
	// Inicjacja wyœwietlacza
	lcd_Init();
	lcd_SetStatus(LCD_STATUS_DISP);
	// Inicjacja timera systemowego
	TCCR2 = 1<<WGM21 /*CTC*/ | 1<<CS22 | 1<<CS21 | 1<<CS20 /*clk:1024*/;
	OCR2 = 78; // oko³o 10ms
	
	// Przerwania
	TIMSK = 1<<OCIE2 /*porównania z timera 2*/;
	sei();

	// Wyœwietlenie zapytania o jêzyk
	fputs_P(PSTR("S1 - "), lcd_GetFile());
	fputs_P(langsys_GetLangName(0), lcd_GetFile());
	lcd_GoTo(0, 1);
	fputs_P(PSTR("S2 - "), lcd_GetFile());
	fputs_P(langsys_GetLangName(1), lcd_GetFile());
	lcd_Update();
	
	// Oczekiwanie na przycisk 
	
	
	// Wywo³anie aplikacji
	app_run();
}

//Funkcje obs³ugi opóŸnienia
inline void system_delaySet(uint_8 delay)
{
	system_delay = delay;
}

inline uint8_t system_delayGet(void)
{
	return system_delay;
}

void system_delayMake(uint8_t delay)
{
	system_delaySet(delay);
	while(0 != system_delayGet()) {}
}

//Funkcja czyœci kolejkê:
void system_msgClear(void)
{
	memset(&system_msg, 0, sizeof(system_msg));
}

//Funkcja dodaje waidomoœæ do kolejki
//Zwraca: 0 jeœli powodzenie. 1 jeœli nie przepe³nienie
uint8_t system_msgPut(uint8_t msg)
{
	uint8_t in = (system_msg.indexIn + 1) % ELEMS(system_msg.quene);
	if(in != system_msg.indexOut)
	{
		system_msg.quene[system_msg.indexIn] = msg;
		system_msg.indexIn = in;
		return 0;
	}
	return 1;
	
//Funkcja pobiera wiadomoœæ z kolejki.
//Zwraca: 0 jeœli brak danych, lub kod wiadomoœci
uint8_t system_msgGet(void)
{
	if(system_msg.indexIn == system_msg.indexOut)
		return 0;
	uint8_t msg = system_msg.quene[system_msg.indexOut];
	system_msg.indexOut = (system_msg.indexOut + 1) %
		ELEMS(system_msg.quene);
		
	return msg;
}

uint8_t system_msgWaitFor(void)
{
	uint8_t msg;
	while(0 == (msg = system_msg())) {};
	
	return msg;
}

//zwraca poprzedni¹ wartoœæ
uint8_t system_kbdSetRepeatSpeed(uint_t rep)
{
	uint8_t rep_last = system_kbdRepeatSpeed;
	
	cli();
	system_kbdRepeatSpeed = rep;
	system_kbd[0].RepeatCnt = rep;
	system_kbd[1].RepeatCnt = rep;
	sei();
	
	return rep_last;
}

// key - indeks zmiennej opisuj¹cej stan przycisku
// mask - maska, zawieraj¹ca 1 tylko tam gdzie znajduje siê przycisk
static void kbd_ProcessKey(uint_t key, uint8_t mask)
{
	struct SYSTEM_KBD_KEY_STATUS* pStat = system_kbd+key;
	if(PIN(SW_PORT) & mask)
	{
		//Jeœli w³aœnie puszczony, test czy nie by³o krótkiego
		//trzymania:
		if(pStat->HoldTime != KBD_HOLDTIME && pStat->HoldTime != 0)
			system_msgPut(IDM_SWUP_SHORT + key);
			//Przycisk puszczony
			pStat->HoldTime = KBD_HOLDTIME;
			pStat->RepeatCnt = system_kbdRepeatSpeed;
		}
		else
		{
			//Przycisk nacisiêty
			//Jeœli drugi raz - przycisk odk³ócony
			if(pStat->HoldTime == KBD_HOLDTIME-1)
				system_msgPut(IDM_SWUP + key);
			//W³aœnie mija czas tzrymania:
			else if(pStat->HoldTime == 1)
			{
				if(system kbdRepeatSpeed != 0)
				system_msgPut(IDM_SWUP_REPEAT + key);
				system_msgPut(IDM_SWUP_HOLD + key);
			}
			
			if(pStat->HoldTime != 0)
				-(pStat->HoldTime);
			else if(system_kbdRepeatSpeed != 0)
			//Mamy do czynienia z trzymaniem, badamy repetycjê:
			{
				if(pStat->RepeatCnt == 0)
				{
					system_msgPut(IDM_SWUP_REPEAT + key);
					pStat->RepeatCnt = system_kbdRepeatSpeed;
				}
				else
					-(pStat->RepeatCnt);
			}
		}
}

static inline void system_kbdMakeScan(void)
{
	kbd_ProcessKey(KBD_UP, 1<<SW1);
	kbd_ProcessKey(KBD_DOWN, 1<<SW2);
}

ISR(TIMER2_COMP_vect)
{
	static uint8_t ticks = 0;
	//przyspieszenie dzia³ania volatile
	uint_8 delay = system_delay;
	//co 40ms przerwanie klawiatury
	if(ticks % 4 == 0)
		system_kbdMakeScan();
	
	for(a=0; a<ELEMS(system_timer); a++)
	{
		struct SYSTEM_TIMER* pT = system_timer + a;
		if(pT->mode == SYSTEM_TIMERMODEONESHOT)
		{
			if(pT->cnt != 0)
			{
				-(pT->cnt);
				if(pT->cnt == 0)
					system_msgPut(IDM_TIMER0+a);
			}
		}
		else if(pT->mode == SYSTEM_TIMER_MODEFREERUN)
		{
			if(pt->cnt == 0)
			{
				pT->cnt = pT->start;
				system_msgPut(IDM_TIMER0+a);
			}
			else
				-(pT->cnt);
		}
	}
	
	//Generowanie opóŸnieñ
	if(delay != 0)
	{
		--delay;
		system_delay = delay;
	}
	//Zliczanie kolejnego przerwania
	++ticks;
}

void system_timerSet(uint8_t timer, uint8_t mode, uint8_t start, uint8_t init)
{
	//Zabezpieczenie przed przerwaniami
	cli();
	//Inicjacja timera
	system_timer[timer].start = start;
	system_timer[timer].cnt = init;
	system_timer[timer].mode = mode;
	sei()
}

inline void system_timerReset(uint8_t timer)
{
	cli();
	system_timer[timer].cnt = system_timer[timer].start;
	sei();
}

