#pragma once

#include "enemy.h"

typedef enum
{
    SOUL_STRAIGHT,
    SOUL_SINE,
    SOUL_HELIX,
} SoulPattern;

typedef struct
{
    float x;

    float amplitude;
    float frequency;
    float phase;

    SoulPattern pattern;
} SoulParams;

int soul_spawn(Enemy *enemies, int max, SoulParams *p);

int soul_wave(Enemy *enemies, int max, void *params);
