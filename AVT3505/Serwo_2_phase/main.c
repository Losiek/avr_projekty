#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include "harddef.h"
#include "Silnik_krokowy.c"

void single_stepping(uint8_t direct, uint8_t stepps, uint8_t speed) 
{
	if (direct == 0) {
		for(int a = 0; a<stepps; ++a) {
			Serwo_PORT |= 1<<A1;
			_delay_ms(speed);
			Serwo_PORT = ~(0xff);
			Serwo_PORT |= 1<<B1;
			_delay_ms(speed);
			Serwo_PORT = ~(0xff);
			Serwo_PORT |= 1<<A2;
			_delay_ms(speed);
			Serwo_PORT = ~(0xff);
			Serwo_PORT |= 1<<B2;
			_delay_ms(speed);
			Serwo_PORT = ~(0xff);
			}
		}
	else {
		for(int a = 0; a<stepps; ++a) {
			Serwo_PORT |= 1<<B2;
			_delay_ms(speed);
			Serwo_PORT = ~(0xff);
			Serwo_PORT |= 1<<A2;
			_delay_ms(speed);
			Serwo_PORT = ~(0xff);
			Serwo_PORT |= 1<<B1;
			_delay_ms(speed);
			Serwo_PORT = ~(0xff);
			Serwo_PORT |= 1<<A1;
			_delay_ms(speed);
			Serwo_PORT = ~(0xff);
			}
		}
	}

void high_torque_stepping(uint8_t direct, uint8_t stepps, uint8_t speed) 
{
	if (direct == 0) {
		for(int a = 0; a<stepps; ++a) {
			Serwo_PORT |= 1<<A1;
			Serwo_PORT |= 1<<B1;
			_delay_ms(speed);
			Serwo_PORT = ~(0xff);
			Serwo_PORT |= 1<<B1;
			Serwo_PORT |= 1<<A2;
			_delay_ms(speed);
			Serwo_PORT = ~(0xff);
			Serwo_PORT |= 1<<A2;
			Serwo_PORT |= 1<<B2;
			_delay_ms(speed);
			Serwo_PORT = ~(0xff);
			Serwo_PORT |= 1<<B2;
			Serwo_PORT |= 1<<A1;
			_delay_ms(speed);
			Serwo_PORT = ~(0xff);
			}
		}
	else {
		for(int a = 0; a<stepps; ++a) {
			Serwo_PORT |= 1<<B2;
			Serwo_PORT |= 1<<A1;
			_delay_ms(speed);
			Serwo_PORT = ~(0xff);
			Serwo_PORT |= 1<<A2;
			Serwo_PORT |= 1<<B2;
			_delay_ms(speed);
			Serwo_PORT = ~(0xff);
			Serwo_PORT |= 1<<B1;
			Serwo_PORT |= 1<<A2;
			_delay_ms(speed);
			Serwo_PORT = ~(0xff);
			Serwo_PORT |= 1<<A1;
			Serwo_PORT |= 1<<B1;
			_delay_ms(speed);
			Serwo_PORT = ~(0xff);
			}
		}
	}

int main(void) {
	//Inicjalizacja
	Serwo_DDR = 1<<A1 | 1<<A2 | 1<<B1 | 1<<B2;
	Serwo_PORT = ~(1<<A1 | 1<<A2 | 1<<B1 | 1<<B2);
	//Koniec inicjacji

	single_stepping(1,12,5);
	single_stepping(0,12,5);
	single_stepping(1,12,5);
	single_stepping(0,12,5);
	single_stepping(1,12,5);
	single_stepping(0,12,5);

	high_torque_stepping(1, 24, 10);
	high_torque_stepping(0, 24, 10);
	high_torque_stepping(1, 24, 10);
	high_torque_stepping(0, 24, 10);
	high_torque_stepping(1, 24, 10);
	
	single_stepping(1,12,50);
	single_stepping(1,12,40);
	single_stepping(1,12,30);
	single_stepping(1,12,20);
	single_stepping(1,12,15);
	single_stepping(1,12,10);
	single_stepping(1,12,5);
	single_stepping(1,12,4);
	single_stepping(1,12,3);
	single_stepping(1,12,2);
	single_stepping(1,12,2);
	single_stepping(1,12,2);
	single_stepping(1,240,2);
	return 0;
	}

