#ifndef I2C_H_INCLUDED
#define I2C_H_INCLUDED

#define I2C_ACK 1
#define I2C_NACK 0

void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_send(uint8_t data);
uint8_t i2c_get(uint8_t ack);

#endif //I2C_H_INCLUDED
