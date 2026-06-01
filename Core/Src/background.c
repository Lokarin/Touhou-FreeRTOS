#include "background.h"
#include "display.h"
#include "common.h"
#include <stddef.h>

static uint8_t bg_buf[SCREEN_W * SCREEN_H * 2];
const uint16_t *g_background = NULL;

void background_set(const uint16_t *bg)
{
    g_background = bg;
}

void background_draw_region(int x, int y, int w, int h)
{
    if (g_background == NULL) return;

    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }
    if (x + w > SCREEN_W) w = SCREEN_W - x;
    if (y + h > SCREEN_H) h = SCREEN_H - y;
    if (w <= 0 || h <= 0) return;

    for (int row = 0; row < h; row++)
    {
        for (int col = 0; col < w; col++)
        {
            uint16_t pixel = g_background[(y + row) * SCREEN_W + (x + col)];
            int idx = (row * w + col) * 2;
            bg_buf[idx]     = pixel & 0xFF;
            bg_buf[idx + 1] = pixel >> 8;
        }
    }

    Display_DrawRawBuffer(x, y, w, h, bg_buf);
}
