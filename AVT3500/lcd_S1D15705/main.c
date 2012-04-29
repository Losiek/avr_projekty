//Sterowanie wyœwietlaczem LCD ze sterownikiem S1D15705

#include <avr\io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "harddef.h"
#include "lcd_S1D15705.h"

int main(void) {
	////////////////////////////////////////
	// inicjacja
	lcd_init();
	lcd_write_c(0xab);
	lcd_write_c(0xa3);
	//lcd_write_c(0xa7);
	lcd_write_c(0xc8);
	lcd_write_c(0x40);
	lcd_write_c(0x2b);
	lcd_write_c(0x26);
	lcd_write_c(0x81);
	lcd_write_c(0x13);
	lcd_write_c(0xaf);
	lcd_write_c(0xa5);
	lcd_write_c(0xa4);
	lcd_init();
	
	lcd_dataDDR = 0x00;
	lcd_read_reg();
	// koniec inicjacji
	///////////////////////////////////////
	return 0;
	}
	