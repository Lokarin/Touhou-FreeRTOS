#include "sprites.h"
#include "renderer.h"

// sprites.c
#include "background.h"
#include "common.h"

// sprites.c
static uint8_t sprite_buf[64 * 64 * 2];  // suporta sprites até 64x64

void sprite_draw(const Sprite *sprite, int x, int y, int flip_h)
{
    // usa sprite_buf em vez de VLA
    for (int row = 0; row < sprite->height; row++)
    {
        for (int col = 0; col < sprite->width; col++)
        {
            int src_col = flip_h ? (sprite->width - 1 - col) : col;
            uint16_t pixel = sprite->data[row * sprite->width + src_col];

            if (pixel == SPRITE_TRANSPARENT)
            {
                int screen_x = x + col;
                int screen_y = y + row;
                if (g_background != NULL &&
                    screen_x >= 0 && screen_x < SCREEN_W &&
                    screen_y >= 0 && screen_y < SCREEN_H)
                    pixel = g_background[screen_y * SCREEN_W + screen_x];
                else
                    pixel = 0x0000;
            }

            int idx = (row * sprite->width + col) * 2;
            sprite_buf[idx]     = pixel & 0xFF;
            sprite_buf[idx + 1] = pixel >> 8;
        }
    }

    Display_DrawSprite(x, y, sprite->width, sprite->height, sprite_buf);
}
