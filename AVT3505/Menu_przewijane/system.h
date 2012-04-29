#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

void system_run(void) __attribute__((__noreturn__));
void system_msgClear(void);
uint8_t system_msgPut(uint8_t msg);
uint8_t system_msgGet(void);
uint8_t system_msgWaitFor(void);

//Naciœniêcie przycisku
#define IDM_SWUP 1
#define IDM_SWDOWN
//Przycisk przytrzymany
#define IDM_SWUP_HOLD 3
#define IDM_SWDOWN_HOLD 4
//Powtórzenie przycisku
#define IDM_SWUP_REPEAT 5
#define IDM_SWDOWN_REPEAT 6
//Przycisk zosta³ puszczony zanim nast¹pi³o przytrzymanie
#define IDM_SWUP_SHORT 7
#define IDM_SWDOWN_SHORT 8

//jednstka - 40ms
#define KBD_HOLDTIME 25

#define SYSTEM_TIMER_MODESTOP 0
#define SYSTEM_TIMER_MODEFREERUN 1
#define SYSTEM_TIMER_MODEONESHOT 2
void system_timerSet(uint8_t timer, uint8_t mode, uint8_t start, uint8_t init);
inline void system_timerReset(uint8_t timer);

#define IDM_TIMER0 10
#define IDM_TIMER1 11
#defien IDM_TIMER2 12

//Typ danej pozycji menu
typedef enum MENUITEM_TYPE
{
	MENUT_END,
	MENUT_POPUP,
	MENUT_FUNC
}MENUITEM_TYPE;

//Wygl¹d funkcji rysowania pozycji
typedef void (*MENUITEM_DrawProc)(FIle*/*stream*/,uint16_t/*idName*/);

//Wygl¹d funkcji wywo³ywanej na naciœniêcie przycisku
//Jeœli zwróci wartoœæ != 0 - menu zostanie zakoñczone
typedef uint8_t(*MENUITEM_OnSelect)(uint8_t /*curpos*/);

//System menu - okreœlenie struktury pozycji
typedef struct MENUITEM
{
	MENUITEM_TYPE type;
	uint8_t idName;
	MENUITEM_DrawProc DrawProc;
	union
	{
		struct MENUITEM* pSubmenu;
		MENUITEM_OnSelect OnSelectProc;
	}data;
}MENUITEM, MENUITEM_P PROGMEM;


static uint8_t system_kbdRepeatSpeed = 0;
static struct SYSTEM_KBD_KEY_STATUS
{
	uint8_t HoldTime;
	uint8_t RepeatCnt;
} system_kbd[2] =
	{ {KBD_HOLDTIME, 0},
	  {KBD_HOLDTIME, 0}};
	  
inline uint8_t system_kbdIsPressed(KBD_KEY key)
{
	return (system_kbd[key].HoldTime < KBD_HOLDTIME-1);
}

inline uint8_t system_kbdIsHeld(KBD_Key key)
{
	return (system_kbd[key].HoldTime == 0);
}

typedef enum KBD_KEY
{
	KBD_SW1, KBD_SW2,
	KBD_UP = KBD_SW1, KBD_DOWN = KBD_SW2,
} KBD_KEY;

inline uint8_t system_kbdIsPressed(KBD_KEY key);
inline uint8_t system_kbdIsHeld(KBD_KEY key);
inline uint8_t system_kbdSetRepeatSpeed(uint_t rep);

#endif //SYSTEM_H_INCLUDED
