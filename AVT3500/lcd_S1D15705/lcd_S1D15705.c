#include <avr\io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "harddef.h"
#include "lcd_S1D15705.h"

//Biblioteka obs³ugi wyœwietlacza ze sterownikiem S1D15705
//////////////////////////////////////////////////////////
void lcd_init(void) {
	lcd_commandDDR = (1<<A0 | 1<<RD | 1<<WR | 1<<RESET | 1<<CS1);
	lcd_commandport = 0x00;
	
	lcd_dataDDR = 0xff;
	lcd_dataport = 0x00;
	}

void lcd_read_data() {
	lcd_commandport = 1<<A0 | ~(1<<RD) | 1<<WR;
	}
	
void lcd_write_data() {
	lcd_commandport = 1<<A0 | 1<<RD | ~(1<<WR);
	}
	
void lcd_read_reg() {
	lcd_commandport = ~(1<<A0 | 1<<RD) | 1<<WR;
	}
	
void lcd_write_command() {
	lcd_commandport = 1<<RD;
	_delay_loop_1(2);
	}
	
void lcd_write_c(uint8_t command) {
	lcd_write_command();
	lcd_dataport = command;
	_delay_loop_1(3);
	lcd_commandport = 1<<A0 | 1<<CS1;
	}
	
void lcd_write_d(uint8_t dana) {
	lcd_write_data();
	lcd_dataport = dana;
	lcd_commandport = 1<<CS1;
	}