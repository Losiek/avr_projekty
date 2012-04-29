#ifndef 1WIRE_H_INCLUDED
#define 1WIRE_H_INCLUDED

void delay(uint8_t t);
void ow_reset(void);
void ow_write_bit(uint8_t b);
uint8_t ow_read_bit(void);
uint8_t ow_read_byte(void);
void ow_write_byte(char val);

#endif //HARDDEF_H_INCLUDED