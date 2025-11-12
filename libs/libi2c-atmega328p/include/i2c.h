//
// Created by Adjy Sedar Desir on 28/10/2025.
//

#ifndef I2C_LIB_ATMEGA32U4_I2C_H
#define I2C_LIB_ATMEGA32U4_I2C_H

#include <stdint.h>
#ifndef F_SCL
#define F_SCL 100000UL
#endif

#ifndef F_CPU
#warning "F_CPU should be defined by the build system (e.g. -DF_CPU=16000000UL)"
#endif

#define TWBR_val ((((F_CPU / F_SCL) - 16 ) / 2 ))

void i2c_init(void);
void i2c_start_write(uint8_t address7);
void i2c_write(uint8_t data);
void i2c_stop(void);
uint8_t i2c_read(uint8_t ack);
void i2c_start_read(uint8_t address);


#endif //I2C_LIB_ATMEGA32U4_I2C_H
