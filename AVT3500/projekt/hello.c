#include <avr/io.h>
#include <avr/delay.h>

int main(void) 
{
	//Ustawienie wyj��
	DDRD = 0x7c;
	
	for(;;)
	{
		//wy��cznie diod
		PORTD = 0x7c;
		_delay_loop_2(0xffff);
		PORTD = 0;
		_delay_loop_2(0xffff);
	}
	return 0;
}
