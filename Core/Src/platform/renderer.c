#include "renderer.h"
#include "display.h"
#include "common.h"

void renderer_draw_rect(int x, int y, int w, int h, unsigned int color)
{
    // Clipa contra os limites do display
    if (x >= SCREEN_W || y >= SCREEN_H) return;
    if (x + w <= 0   || y + h <= 0)    return;

    int cx = x < 0 ? 0 : x;
    int cy = y < 0 ? 0 : y;
    int cw = (x + w > SCREEN_W ? SCREEN_W : x + w) - cx;
    int ch = (y + h > SCREEN_H ? SCREEN_H : y + h) - cy;

    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8)  & 0xFF;
    uint8_t b = (color)       & 0xFF;
    uint32_t rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);

    Display_FillRect(cx, cy, cw, ch, rgb565);
}
