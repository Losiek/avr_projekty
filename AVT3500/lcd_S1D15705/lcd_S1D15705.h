#ifndef lcd_S1D15705_H
#define lcd_S1D15705_H

void lcd_init(void);
void lcd_read_data(void);
void lcd_write_data(void);
void lcd_read_reg(void);
void lcd_write_command(void);

void lcd_write_c(uint8_t command);
void lcd_write_d(uint8_t dana);
void lcd_read(void);

#endif //lcd_S1D1570_H