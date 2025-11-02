//
// Created by Adjy Sedar Desir on 28/10/2025.
//

#include "pn532.h"

#ifndef PN532_LIB_ATMEGA32U4_PN532_I2C_H
#define PN532_LIB_ATMEGA32U4_PN532_I2C_H


#define PN532_I2C_ADDRESS                   (0x48 >> 1)
#define PN532_I2C_READBIT                   (0x01)
#define PN532_I2C_BUSY                      (0x00)
#define PN532_I2C_READY                     (0x01)
#define PN532_I2C_READYTIMEOUT              (20)

void PN532_I2C_Init(PN532* pn532);

#endif //PN532_LIB_ATMEGA32U4_PN532_I2C_H
