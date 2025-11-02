//
// Created by Adjy Sedar Desir on 25/10/2025.
//

#include "../include/ssd1306_text.h"
#include <stddef.h>
#include <stdbool.h>
#include "../include/ssd1306.h"
#include "../include/font5x7.h"

#define GLYPH_WIDTH 5
#define GLYPH_SPACING 1
#define GLYPH_PX (GLYPH_WIDTH + GLYPH_SPACING)

// Draw a 5x7 glyph on the screen
static void draw_glyph5x7(const uint8_t glyph[GLYPH_WIDTH]) {
    for (uint8_t i = 0; i < GLYPH_WIDTH; ++i) {
        ssd1306_send_data_byte(glyph[i]);
    }
    ssd1306_send_data_byte(0x00); // 1px spacing
}

// Decode next glyph from a limited UTF-8 set (ASCII included)
// Returns number of bytes consumed and updates *out
static uint8_t next_glyph(const char *s, const uint8_t **out) {
    unsigned char c = (unsigned char)s[0];
    if (c == 0x00) {
        *out = NULL;
        return 0;
    }

    // Handle accented UTF-8 letters
    if (c == 0xC3 && s[1]) {
        unsigned char t = (unsigned char)s[1];
        switch (t) {
            case 0xA9: *out = font_accent[0]; return 2; // é
            case 0x88: *out = font_accent[1]; return 2; // è
            case 0x80: *out = font_accent[2]; return 2; // à
            case 0x94: *out = font_accent[3]; return 2; // ô
            case 0x99: *out = font_accent[4]; return 2; // ù
            case 0xA7: *out = font_accent[5]; return 2; // ç
            default:
                *out = font5x7['?' - FONT5X7_FIRST];
                return 2;
        }
    }

    // Treat all other characters (including ASCII)
    if (c < FONT5X7_FIRST || c > FONT5X7_LAST) c = '?';
    *out = font5x7[c - FONT5X7_FIRST];
    return 1;
}

// Measure the width in pixels of a string (up to '\n')
uint16_t measure_string_px(const char *s) {
    uint16_t count = 0;
    while (*s && *s != '\n') {
        const uint8_t *glyph = NULL;
        uint8_t consumed = next_glyph(s, &glyph);
        if (!consumed) break;
        count++;
        s += consumed;
    }
    return count * GLYPH_PX;
}

static void print_string_core(const char *s, uint8_t start_page, uint8_t start_col,
                              uint16_t max_width_px, bool center, uint16_t scroll_px) {
    if (!s || start_page >= SSD1306_PAGES) return;

    uint16_t base_col = start_col < SSD1306_WIDTH ? start_col : 0;
    uint16_t line_right = max_width_px ? base_col + max_width_px : SSD1306_WIDTH;
    if (line_right > SSD1306_WIDTH) line_right = SSD1306_WIDTH;

    uint8_t page = start_page;
    uint16_t col = base_col;

    while (*s && page < SSD1306_PAGES) {
        // Handle new line
        if (*s == '\n') {
            page++;
            if (page >= SSD1306_PAGES) break;
            col = base_col;
            ssd1306_set_cursor(page, (uint8_t)col);
            s++;
            continue;
        }

        const uint8_t *glyph = NULL;
        uint8_t consumed = next_glyph(s, &glyph);
        if (!consumed) break;

        // Scroll adjustment
        if (scroll_px >= GLYPH_PX) {
            scroll_px -= GLYPH_PX;
            s += consumed;
            continue;
        }

        // Wrap to next line if glyph exceeds line width
        if ((col + GLYPH_PX) > line_right) {
            page++;
            if (page >= SSD1306_PAGES) break;
            col = base_col;
            ssd1306_set_cursor(page, (uint8_t)col);
        }

        // Center adjustment (ignored if scrolling)
        if (center && scroll_px == 0 && col == base_col) {
            uint16_t remaining = 0;
            const char *line_start = s;
            while (*line_start && *line_start != '\n') {
                const uint8_t *tmp_glyph = NULL;
                uint8_t tmp_consumed = next_glyph(line_start, &tmp_glyph);
                if (!tmp_consumed) break;
                remaining += GLYPH_PX;
                line_start += tmp_consumed;
            }
            uint16_t draw_w = (remaining > (line_right - base_col)) ? (line_right - base_col) : remaining;
            col += ((line_right - base_col) - draw_w) / 2u;
            ssd1306_set_cursor(page, (uint8_t)col);
        }

        draw_glyph5x7(glyph);
        col += GLYPH_PX;
        s += consumed;
    }
}



// Public functions

// Print UTF-8 string with optional max width and optional scroll offset
void ssd1306_print_utf8_with_max_width_scroll(const char *s, uint8_t start_page, uint8_t start_col,
                                              uint8_t max_width_px, uint16_t scroll_px) {
    print_string_core(s, start_page, start_col, max_width_px, false, scroll_px);
}

// Print UTF-8 string across the full width
void ssd1306_print_utf8(const char *s, uint8_t start_page, uint8_t start_col) {
    print_string_core(s, start_page, start_col, 0, false, 0);
}

// Print UTF-8 string centered on the page
void ssd1306_print_utf8_center(const char *s, uint8_t page) {
    print_string_core(s, page, 0, 0, true, 0);
}
// Print UTF-8 string with optional max width
void ssd1306_print_utf8_with_max_width(const char *s, uint8_t start_page, uint8_t start_col, uint8_t max_width_px) {
    print_string_core(s, start_page, start_col, max_width_px, false, 0);
}


// Print a single character at a specific page/column
void ssd1306_putc(char c, uint8_t page, uint8_t col) {
    const uint8_t *glyph = NULL;
    char s[2] = {c, 0};
    next_glyph(s, &glyph);
    ssd1306_set_cursor(page, col);
    draw_glyph5x7(glyph);
}

// Scroll a long UTF-8 string horizontally on a single line
// scroll_offset: number of pixels to shift to the left
void ssd1306_print_utf8_scroll(const char *s, uint8_t page, uint8_t start_col,
                               uint8_t max_width_px, uint16_t scroll_offset) {
    if (!s || page >= SSD1306_PAGES) return;

    uint16_t base_col = start_col < SSD1306_WIDTH ? start_col : 0;
    uint16_t line_right = base_col + max_width_px;
    if (line_right > SSD1306_WIDTH) line_right = SSD1306_WIDTH;

    // Clear only this line/page
    ssd1306_set_cursor(page, 0);
    for (uint16_t i = 0; i < SSD1306_WIDTH; i++) {
        ssd1306_send_data_byte(0x00);
    }

    uint16_t col = base_col;
    const char *p = s;
    uint16_t skipped_px = 0;

    ssd1306_set_cursor(page, (uint8_t)col);

    while (*p && col < line_right) {
        const uint8_t *glyph = NULL;
        uint8_t consumed = next_glyph(p, &glyph);
        if (!consumed) break;

        // Skip pixels until scroll_offset is reached
        if (skipped_px + GLYPH_PX <= scroll_offset) {
            skipped_px += GLYPH_PX;
            p += consumed;
            continue;
        }

        // Draw visible glyph
        draw_glyph5x7(glyph);
        col += GLYPH_PX;
        p += consumed;
    }
}


