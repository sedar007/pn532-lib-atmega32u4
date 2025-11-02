//
// Created by Adjy Sedar Desir on 25/10/2025.
//

#ifndef SSD1306_AVR_SSD1306_H
#define SSD1306_AVR_SSD1306_H

#pragma once
#include <stdint.h>
#include <stddef.h>

#define SSD1306_I2C_ADDR 0x3C
#define SSD1306_CTRL_CMD 0x00
#define SSD1306_CTRL_DATA 0x40

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define SSD1306_PAGES (SSD1306_HEIGHT/8)

void ssd1306_init(void);
void ssd1306_send_command(uint8_t cmd);
void ssd1306_send_data_byte(uint8_t data);
void ssd1306_send_data(const uint8_t *buf, size_t len);
void ssd1306_set_cursor(uint8_t page, uint8_t col); // page: 0..7, col: 0..127
void ssd1306_clear(void);

#endif //SSD1306_AVR_SSD1306_H
