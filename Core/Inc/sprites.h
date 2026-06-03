#pragma once

#include <stdint.h>
#include "display.h"

#define SPRITE_TRANSPARENT 0xF81F  // magenta RGB565

typedef struct {
    uint8_t         width;
    uint8_t         height;
    const uint16_t *data;
} Sprite;

/* Sprites do player */
extern const Sprite player_static;
extern const Sprite player_movement;
extern const Sprite windmill_1;
extern const Sprite windmill_2;
extern const Sprite player_bullet;
extern const Sprite bullet_green;
extern const Sprite bullet_green_small;
extern const Sprite text_start;
extern const Sprite text_easy;
extern const Sprite text_medium;
extern const Sprite text_lunatic;
extern const Sprite select_cursor;
extern const Sprite fairy_1;
extern const Sprite fairy_2;
extern const Sprite soul_1;
extern const Sprite soul_2;
extern const Sprite small_white_bullet;


/* Desenha um sprite na posição (x, y).
   flip_h = 1 espelha horizontalmente (para andar à esquerda) */
void sprite_draw(const Sprite *sprite, int x, int y, int flip_h);
