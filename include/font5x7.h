//
// Created by Adjy Sedar Desir on 25/10/2025.
//

#ifndef SSD1306_AVR_FONT5X7_H
#define SSD1306_AVR_FONT5X7_H

#pragma once
#include <stdint.h>

extern const uint8_t font5x7[][5];
extern const uint8_t font_accent[][5];

// Optional helpers to know counts (adjust if needed)
#define FONT5X7_FIRST 32
#define FONT5X7_LAST  126
#define FONT5X7_COLS  5

#endif //SSD1306_AVR_FONT5X7_H
