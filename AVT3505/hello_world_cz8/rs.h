/////////////////////////////////////////////////////////////////////////////////////////////
// rs.h - prosta obs³uga portu szeregowego
//		Przeznaczenie: czêœæ 7 kursu C. P³ytka AVT3505
//
// Autor: Rados³aw Koppel		Kompilator: WinAVR 20050214
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef RS_H_INCLUDED
#define RS_H_INCLUDED

#define RS_MAKE_UBRR(baud) (F_CPU/(baud*16l)-1)
#define RS_SET_BAUD(baud) \
	{UBRR0H = (uint8_t)(RS_MAKE_UBRR(baud)>>8); \
	 UBRR0L = (uint8_t)RS_MAKE_UBRR(baud); }

int rs_put(char znak);
int rs_get(void);

#endif // RS_H_INCLUDED
