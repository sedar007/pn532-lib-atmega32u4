#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "ssd1306/include/ssd1306.h"
#include "ssd1306/include/ssd1306_text.h"
#include "../include/pn532_i2c.h"

int main(void) {
    ssd1306_init();
    ssd1306_clear();
    ssd1306_print_utf8_center("Initialisation ...", 1);

    PN532 pn532;
    uint8_t buff[255];
    PN532_I2C_Init(&pn532);

    if (PN532_GetFirmwareVersion(&pn532, buff) != PN532_STATUS_OK) {
        ssd1306_print_utf8_center("Erreur PN532", 1);
        return -1;
    }
    ssd1306_print_utf8_center("PN532 detecte", 1);

    PN532_SamConfiguration(&pn532);
    ssd1306_print_utf8_center("En attente d'une carte...", 2);

    uint8_t uid[MIFARE_UID_MAX_LENGTH];
    int32_t uid_len = 0;

    for (;;) {
        uid_len = PN532_ReadPassiveTarget(&pn532, uid, PN532_MIFARE_ISO14443A, 1000);
        if (uid_len <= 0) {
            continue;
        }

        // Build UID string
        char uid_str[32];
        uid_str[0] = '\0';
        for (int i = 0; i < uid_len; i++) {
            char tmp[4];
            sprintf(tmp, "%02X", uid[i]);
            strcat(uid_str, tmp);
            if (i < uid_len - 1) strcat(uid_str, " ");
        }

        ssd1306_clear();
        ssd1306_print_utf8_center("Carte detectee!", 1);
        ssd1306_print_utf8_center(uid_str, 2);

        // Try MIFARE Classic first (auth A then B)
        int wrote = 0;
        uint8_t block = 4;  // avoid sector trailers (3, 7, 11, ...)
        uint8_t data16[16] = { 'H','e','l','l','o',' ','A','V','R',' ','P','N','5','3','2','!' };
        uint8_t keyA[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
        uint8_t keyB[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

        int auth = PN532_MifareClassicAuthenticateBlock(&pn532, uid, (uint8_t)uid_len, block, MIFARE_CMD_AUTH_A, keyA);
        if (auth != PN532_ERROR_NONE) {
            auth = PN532_MifareClassicAuthenticateBlock(&pn532, uid, (uint8_t)uid_len, block, MIFARE_CMD_AUTH_B, keyB);
        }

        if (auth == PN532_ERROR_NONE) {
            if (PN532_MifareClassicWriteBlock(&pn532, data16, block) == PN532_ERROR_NONE) {
                ssd1306_print_utf8_center("Write OK", 2);
                wrote = 1;
            } else {
                ssd1306_print_utf8_center("Write error", 2);
            }
        }

        // If not Classic or auth failed, try NTAG/Ultralight 4-byte page write
        if (!wrote) {
            uint8_t page = 4;
            uint8_t payload[4] = {'T','E','S','T'};
            if (PN532_Ntag2xxWriteBlock(&pn532, payload, page) == PN532_ERROR_NONE) {
                ssd1306_print_utf8_center("Ultralight write OK", 2);
            } else {
                ssd1306_print_utf8_center("Auth error / wrong key / tag type", 2);
            }
        }

        printf("Carte detectee - UID: %s\n", uid_str);
        _delay_ms(500);
    }

    return 0;
}
