#include "sprites.h"
#include "renderer.h"

// sprites.c
#include "background.h"
#include "common.h"

void sprite_draw(const Sprite *sprite, int x, int y, int flip_h)
{
    uint8_t buf[sprite->width * sprite->height * 2];

    for (int row = 0; row < sprite->height; row++)
    {
        for (int col = 0; col < sprite->width; col++)
        {
            int src_col = flip_h ? (sprite->width - 1 - col) : col;
            uint16_t pixel = sprite->data[row * sprite->width + src_col];

            if (pixel == SPRITE_TRANSPARENT)
            {
                // pega a cor do fundo nessa posição
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
            buf[idx]     = pixel & 0xFF;
            buf[idx + 1] = pixel >> 8;
        }
    }

    Display_DrawSprite(x, y, sprite->width, sprite->height, buf);
}
