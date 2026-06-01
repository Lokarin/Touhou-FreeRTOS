#ifndef DISPLAY_H
#define DISPLAY_H

#include "st7735.h"
#include <stdint.h>

// Cores RGB565 prontas para usar
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_WHITE   0xFFFF
#define COLOR_BLACK   0x0000
#define COLOR_YELLOW  0xFFE0
#define COLOR_CYAN    0x07FF
#define COLOR_MAGENTA 0xF81F

// Dimensões em portrait
#define LCD_WIDTH   128
#define LCD_HEIGHT  160

void Display_Init(void);
void Display_FillRect(int x, int y, int w, int h, uint32_t color);
void Display_FillBlocking(void);
void Display_FillScreen(uint32_t color);
void Display_DrawSprite(int x, int y, int w, int h, uint8_t *data);
void Display_DrawRawBuffer(int x, int y, int w, int h, uint8_t *buf);

#endif
