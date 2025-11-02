//
// Created by Adjy Sedar Desir on 28/10/2025.
//

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifndef PN532_LIB_ATMEGA32U4_PN532_H
#define PN532_LIB_ATMEGA32U4_PN532_H


#define MIFARE_UID_MAX_LENGTH               MIFARE_UID_TRIPLE_LENGTH
#define MIFARE_UID_TRIPLE_LENGTH            (10)
#define MIFARE_CMD_AUTH_A                   (0x60)
#define MIFARE_CMD_AUTH_B                   (0x61)
#define MIFARE_CMD_READ                     (0x30)
#define MIFARE_CMD_WRITE                    (0xA0)
#define MIFARE_CMD_TRANSFER                 (0xB0)
#define MIFARE_CMD_DECREMENT                (0xC0)
#define MIFARE_CMD_INCREMENT                (0xC1)
#define MIFARE_CMD_STORE                    (0xC2)
#define MIFARE_ULTRALIGHT_CMD_WRITE         (0xA2)
#define MIFARE_UID_SINGLE_LENGTH            (4)
#define MIFARE_UID_DOUBLE_LENGTH            (7)
#define MIFARE_KEY_LENGTH                   (6)
#define MIFARE_BLOCK_LENGTH                 (16)


#define PN532_COMMAND_READGPIO              (0x0C)
#define PN532_COMMAND_WRITEGPIO             (0x0E)
#define PN532_COMMAND_INDATAEXCHANGE        (0x40)
#define PN532_COMMAND_DIAGNOSE              (0x00)
#define PN532_COMMAND_GETFIRMWAREVERSION    (0x02)
#define PN532_COMMAND_GETGENERALSTATUS      (0x04)
#define PN532_COMMAND_READREGISTER          (0x06)
#define PN532_COMMAND_WRITEREGISTER         (0x08)
#define PN532_COMMAND_SETSERIALBAUDRATE     (0x10)
#define PN532_COMMAND_SETPARAMETERS         (0x12)
#define PN532_COMMAND_SAMCONFIGURATION      (0x14)
#define PN532_COMMAND_POWERDOWN             (0x16)
#define PN532_COMMAND_RFCONFIGURATION       (0x32)
#define PN532_COMMAND_RFREGULATIONTEST      (0x58)
#define PN532_COMMAND_INJUMPFORDEP          (0x56)
#define PN532_COMMAND_INJUMPFORPSL          (0x46)
#define PN532_COMMAND_INLISTPASSIVETARGET   (0x4A)
#define PN532_COMMAND_INATR                 (0x50)
#define PN532_COMMAND_INPSL                 (0x4E)
#define PN532_COMMAND_INCOMMUNICATETHRU     (0x42)
#define PN532_COMMAND_INDESELECT            (0x44)
#define PN532_COMMAND_INRELEASE             (0x52)
#define PN532_COMMAND_INSELECT              (0x54)
#define PN532_COMMAND_INAUTOPOLL            (0x60)
#define PN532_COMMAND_TGINITASTARGET        (0x8C)
#define PN532_COMMAND_TGSETGENERALBYTES     (0x92)
#define PN532_COMMAND_TGGETDATA             (0x86)
#define PN532_COMMAND_TGSETDATA             (0x8E)
#define PN532_COMMAND_TGSETMETADATA         (0x94)
#define PN532_COMMAND_TGGETINITIATORCOMMAND (0x88)
#define PN532_COMMAND_TGRESPONSETOINITIATOR (0x90)
#define PN532_COMMAND_TGGETTARGETSTATUS     (0x8A)
#define PN532_PREAMBLE                      (0x00)
#define PN532_STARTCODE1                    (0x00)
#define PN532_STARTCODE2                    (0xFF)
#define PN532_POSTAMBLE                     (0x00)
#define PN532_HOSTTOPN532                   (0xD4)
#define PN532_PN532TOHOST                   (0xD5)
#define PN532_ERROR_NONE                                                (0x00)


#define NTAG2XX_BLOCK_LENGTH                (4)


/* Define */
#define PN532_MIFARE_ISO14443A              (0x00)
#define PN532_STATUS_ERROR                                              (-1)
#define PN532_STATUS_OK                                                 (0)


typedef struct _PN532 {
    int (*reset)(void);
    int (*read_data)(uint8_t* data, uint16_t count);
    int (*write_data)(uint8_t *data, uint16_t count);
    bool (*wait_ready)(uint32_t timeout);
    int (*wakeup)(void);
    void (*log)(const char* log);
} PN532;

int PN532_WriteFrame(PN532* pn532, uint8_t* data, uint16_t length);
int PN532_ReadFrame(PN532* pn532, uint8_t* buff, uint16_t length);
int PN532_CallFunction(PN532* pn532, uint8_t command, uint8_t* response, uint16_t response_length, uint8_t* params, uint16_t params_length, uint32_t timeout);
int PN532_GetFirmwareVersion(PN532* pn532, uint8_t* version);
int PN532_SamConfiguration(PN532* pn532);
int PN532_ReadPassiveTarget(PN532* pn532, uint8_t* response, uint8_t card_baud, uint32_t timeout);
int PN532_MifareClassicAuthenticateBlock(PN532* pn532, uint8_t* uid, uint8_t uid_length, uint16_t block_number, uint16_t key_number, uint8_t* key);
int PN532_MifareClassicReadBlock(PN532* pn532, uint8_t* response, uint16_t block_number);
int PN532_MifareClassicWriteBlock(PN532* pn532, uint8_t* data, uint16_t block_number);
int PN532_Ntag2xxReadBlock(PN532* pn532, uint8_t* response, uint16_t block_number);
int PN532_Ntag2xxWriteBlock(PN532* pn532, uint8_t* data, uint16_t block_number);
int PN532_ReadGpio(PN532* pn532, uint8_t* pins_state);
bool PN532_ReadGpioP(PN532* pn532, uint8_t pin_number);
bool PN532_ReadGpioI(PN532* pn532, uint8_t pin_number);
int PN532_WriteGpio(PN532* pn532, uint8_t* pins_state);
int PN532_WriteGpioP(PN532* pn532, uint8_t pin_number, bool pin_state);


#endif //PN532_LIB_ATMEGA32U4_PN532_H
