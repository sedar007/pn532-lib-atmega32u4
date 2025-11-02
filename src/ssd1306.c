//
// Created by Adjy Sedar Desir on 25/10/2025.
//

#include "../include/ssd1306.h"

#include <util/delay.h>
#include "../include/i2c.h"

static inline void ssd1306_begin_tx(uint8_t control) {
    i2c_start_write(SSD1306_I2C_ADDR);
    i2c_write(control);
}

void ssd1306_send_command(uint8_t cmd) {
    ssd1306_begin_tx(SSD1306_CTRL_CMD);
    i2c_write(cmd);
    i2c_stop();
}

void ssd1306_send_data_byte(uint8_t data) {
    ssd1306_begin_tx(SSD1306_CTRL_DATA);
    i2c_write(data);
    i2c_stop();
}

void ssd1306_send_data(const uint8_t *buf, size_t len) {
    if (!len) return;
    ssd1306_begin_tx(SSD1306_CTRL_DATA);
    for (size_t i = 0; i < len; ++i) i2c_write(buf[i]);
    i2c_stop();
}

void ssd1306_set_cursor(uint8_t page, uint8_t col) {
    if (page >= SSD1306_PAGES) page = 0;
    if (col >= SSD1306_WIDTH) col = 0;
    ssd1306_send_command((uint8_t)(0xB0 | page));
    ssd1306_send_command((uint8_t)(col & 0x0F));
    ssd1306_send_command((uint8_t)(0x10 | (col >> 4)));
}

void ssd1306_clear(void) {
    for (uint8_t page = 0; page < SSD1306_PAGES; ++page) {
        ssd1306_set_cursor(page, 0);
        for (uint8_t col = 0; col < SSD1306_WIDTH; ++col) {
            ssd1306_send_data_byte(0x00);
        }
    }
}

void ssd1306_init(void) {
    i2c_init();
    _delay_ms(100);

    ssd1306_send_command(0xAE);
    ssd1306_send_command(0xD5); ssd1306_send_command(0x80);
    ssd1306_send_command(0xA8); ssd1306_send_command(0x3F);
    ssd1306_send_command(0xD3); ssd1306_send_command(0x00);
    ssd1306_send_command(0x40);
    ssd1306_send_command(0x8D); ssd1306_send_command(0x14);
    ssd1306_send_command(0x20); ssd1306_send_command(0x00);
    ssd1306_send_command(0xA1); ssd1306_send_command(0xC8);
    ssd1306_send_command(0xDA); ssd1306_send_command(0x12);
    ssd1306_send_command(0x81); ssd1306_send_command(0x7F);
    ssd1306_send_command(0xD9); ssd1306_send_command(0xF1);
    ssd1306_send_command(0xDB); ssd1306_send_command(0x40);
    ssd1306_send_command(0xA4); ssd1306_send_command(0xA6);
    ssd1306_send_command(0xAF);
    ssd1306_clear();
}
