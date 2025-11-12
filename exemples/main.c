#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <ctype.h>

#include "libssd1306-atmega328p/include/ssd1306.h"
#include "libssd1306-atmega328p/include/ssd1306_text.h"
#include "../include/pn532_i2c.h"

/* ======================== UTILITAIRES ======================== */

void debug_log(const char* msg) {

}

void display_text(const char* line1, const char* line2, uint16_t delay_ms) {
    ssd1306_clear();
    if (line1) ssd1306_print_utf8_center(line1, 1);
    if (line2) ssd1306_print_utf8_center(line2, 2);
    if (delay_ms) _delay_ms(300);
}

/* ======================== CONVERSION & AFFICHAGE ======================== */

void hex_to_text(const uint8_t* hex_data, uint16_t length, char* text_output) {
    for (uint16_t i = 0; i < length; i++) {
        text_output[i] = (hex_data[i] >= 32 && hex_data[i] <= 126) ? hex_data[i] : '.';
    }
    text_output[length] = '\0';
}

void display_block_data(uint8_t* block_data, uint8_t block_num) {
    char line[32], text_line[17];
    sprintf(line, "Bloc %d:", block_num);
    display_text(line, NULL, 0);

    sprintf(line, "%02X%02X %02X%02X %02X%02X %02X%02X",
            block_data[0], block_data[1], block_data[2], block_data[3],
            block_data[4], block_data[5], block_data[6], block_data[7]);
    ssd1306_print_utf8_center(line, 2);

    sprintf(line, "%02X%02X %02X%02X %02X%02X %02X%02X",
            block_data[8], block_data[9], block_data[10], block_data[11],
            block_data[12], block_data[13], block_data[14], block_data[15]);
    ssd1306_print_utf8_center(line, 3);

    hex_to_text(block_data, 16, text_line);
    snprintf(line, sizeof(line), "Txt: %.12s", text_line);
    ssd1306_print_utf8_center(line, 4);
}

/* ======================== ÉCRITURE ======================== */

bool write_text_to_block(PN532* pn532, uint8_t* uid, uint8_t uid_len,
                         uint8_t block_num, uint8_t key_type, uint8_t* key,
                         const char* text) {
    uint8_t data[16];
    memset(data, 0x00, sizeof(data));
    strncpy((char*)data, text, 16);

    if (PN532_MifareClassicAuthenticateBlock(pn532, uid, uid_len, block_num, key_type, key) != PN532_ERROR_NONE)
        return false;

    return (PN532_MifareClassicWriteBlock(pn532, data, block_num) == PN532_ERROR_NONE);
}

/* ======================== LECTURE ======================== */

bool read_block(PN532* pn532, uint8_t* uid, uint8_t uid_len,
                uint8_t block_num, uint8_t key_type, uint8_t* key,
                uint8_t* block_data) {
    if (PN532_MifareClassicAuthenticateBlock(pn532, uid, uid_len, block_num, key_type, key) != PN532_ERROR_NONE)
        return false;

    return (PN532_MifareClassicReadBlock(pn532, block_data, block_num) == PN532_ERROR_NONE);
}

/* ======================== INITIALISATION ======================== */

bool init_pn532(PN532* pn532, uint8_t* fw_buff) {
    pn532->log = debug_log;
    PN532_I2C_Init(pn532);

    if (PN532_GetFirmwareVersion(pn532, fw_buff) != PN532_STATUS_OK)
        return false;

    if (PN532_SamConfiguration(pn532) != PN532_STATUS_OK)
        return false;

    return true;
}

/* ======================== DÉTECTION ======================== */

int32_t detect_card(PN532* pn532, uint8_t* uid) {
    return PN532_ReadPassiveTarget(pn532, uid, PN532_MIFARE_ISO14443A, 1000);
}

/* ======================== PROGRAMME PRINCIPAL ======================== */

int main(void) {
    ssd1306_init();
    display_text("Initialisation...", NULL, 500);

    PN532 pn532;
    uint8_t fw_buff[255];
    if (!init_pn532(&pn532, fw_buff)) {
        display_text("Erreur PN532", NULL, 0);
        while (1);
    }

    char fw_str[20];
    sprintf(fw_str, "FW v%d.%d", fw_buff[1], fw_buff[2]);
    display_text("PN532 detecte", fw_str, 500);

    uint8_t uid[MIFARE_UID_MAX_LENGTH];
    uint8_t key_default[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    //uint8_t key_default[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Clé zéro
   // uint8_t key_default[6] = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5}; // Clé MAD
   // uint8_t key_default[6] = {0xD3,0xF7,0xD3,0xF7,0xD3,0xF7};




    for (;;) {
        display_text("Approchez carte", NULL, 0);
        int32_t uid_len = detect_card(&pn532, uid);
        if (uid_len <= 0) {
            _delay_ms(300);
            continue;
        }

        // Affichage UID
        char uid_str[32] = {0};
        for (int i = 0; i < uid_len; i++) {
            char tmp[4];
            sprintf(tmp, "%02X", uid[i]);
            strcat(uid_str, tmp);
        }
        display_text("Carte detectee", uid_str, 500);

        uint8_t key_type = MIFARE_CMD_AUTH_A;
        uint8_t block_data[16];
        uint8_t block_to_use = 5;
        const char* message = "salut";

        // Écriture
        display_text("Ecriture...", NULL, 0);
        if (write_text_to_block(&pn532, uid, uid_len, block_to_use, key_type, key_default, message)) {
            display_text("Ecriture OK!", NULL, 1000);
        } else {
            display_text("Erreur ecriture", NULL, 1000);
        }

        // Lecture de vérification
        if (read_block(&pn532, uid, uid_len, block_to_use, key_type, key_default, block_data)) {
            display_text("Lecture OK", NULL, 500);
            display_block_data(block_data, block_to_use);
        } else {
            display_text("Erreur lecture", NULL, 1000);
        }

        _delay_ms(2000);
    }

    return 0;
}
