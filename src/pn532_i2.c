//
// Created by Adjy Sedar Desir on 28/10/2025.
//

#include <util/delay.h>
#include <stdbool.h>
#include <stdint.h>
#include "pn532_i2c.h"
#include "../libs/libi2c-atmega328p/include/i2c.h"



/*static void PN532_Log(const char* msg) {

}*/

int PN532_I2C_ReadData(uint8_t* data, uint16_t count) {
    i2c_start_read(PN532_I2C_ADDRESS);
    uint8_t status = i2c_read(1); // read status + ACK
    if (status != PN532_I2C_READY) {
        i2c_stop();
        return PN532_STATUS_ERROR;
    }

    for (uint16_t i = 0; i < count; i++) {
        data[i] = i2c_read(i < (count - 1));
    }
    i2c_stop();
    return PN532_STATUS_OK;
}

int PN532_I2C_WriteData(uint8_t* data, uint16_t count) {
    i2c_start_write(PN532_I2C_ADDRESS);
    for (uint16_t i = 0; i < count; i++) {
        i2c_write(data[i]);
    }
    i2c_stop();
    return PN532_STATUS_OK;
}

bool PN532_I2C_WaitReady(uint32_t timeout_ms) {
    uint8_t status;
    uint32_t waited = 0;

    while (waited < timeout_ms) {
        i2c_start_read(PN532_I2C_ADDRESS);
        status = i2c_read(0);
        i2c_stop();
        if (status == PN532_I2C_READY) {
            return true;
        }
        _delay_ms(5);
        waited += 5;
    }
    return false;
}

int PN532_I2C_Wakeup(void) {
    uint8_t wakeup[] = {0x55, 0x55, 0x00, 0x00, 0x00};
    i2c_start_write(PN532_I2C_ADDRESS);
    for (uint8_t i = 0; i < sizeof(wakeup); i++) {
        i2c_write(wakeup[i]);
    }
    i2c_stop();
    _delay_ms(100);
    return PN532_STATUS_OK;
}

int PN532_Reset(void) {
    _delay_ms(400);
    return PN532_STATUS_OK;
}

void PN532_I2C_Init(PN532* pn532) {
    i2c_init();

    pn532->reset = PN532_Reset;
    pn532->read_data = PN532_I2C_ReadData;
    pn532->write_data = PN532_I2C_WriteData;
    pn532->wait_ready = PN532_I2C_WaitReady;
    pn532->wakeup = PN532_I2C_Wakeup;
   // pn532->log = PN532_Log;

    pn532->reset();
    pn532->wakeup();
}
