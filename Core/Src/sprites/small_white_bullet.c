#include "sprites.h"

static const uint16_t _small_white_bullet_data[] = {
    0xF81F, 0xFFFF, 0xFFFF, 0xF81F, 0xFFFF, 0xDEFB, 0xDEFB, 0xFFFF,
    0xFFFF, 0xDEFB, 0xDEFB, 0xFFFF, 0xF81F, 0xFFFF, 0xFFFF, 0xF81F

};

const Sprite small_white_bullet = {
    .width  = 4,
    .height = 4,
    .data   = _small_white_bullet_data,
};
