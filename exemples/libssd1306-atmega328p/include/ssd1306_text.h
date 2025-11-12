//
// Created by Adjy Sedar Desir on 25/10/2025.
//

#ifndef SSD1306_AVR_SSD1306_TEXT_H
#define SSD1306_AVR_SSD1306_TEXT_H
#pragma once
#include <stdint.h>

// Print UTF-8 string with optional max width and optional scroll offset
void ssd1306_print_utf8_with_max_width_scroll(const char *s, uint8_t start_page, uint8_t start_col,
                                              uint8_t max_width_px, uint16_t scroll_px);

// Print UTF-8 string across the full width
void ssd1306_print_utf8(const char *s, uint8_t start_page, uint8_t start_col);

// Print UTF-8 string with optional max width
void ssd1306_print_utf8_with_max_width(const char *s, uint8_t start_page, uint8_t start_col, uint8_t max_width_px);

// Print UTF-8 string centered on the page
void ssd1306_print_utf8_center(const char *s, uint8_t page) ;
// Print a single character at a specific page/column
void ssd1306_putc(char c, uint8_t page, uint8_t col);

void ssd1306_print_utf8_scroll(const char *s, uint8_t page, uint8_t start_col,
                               uint8_t max_width_px, uint16_t scroll_offset);
uint16_t measure_string_px(const char *s);

#endif //SSD1306_AVR_SSD1306_TEXT_H
