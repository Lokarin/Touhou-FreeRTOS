#pragma once

#include "enemy.h"

int soul_spawn(Enemy *enemies, int max, float x);

typedef struct
{
    float x;
} SoulParams;

int soul_wave(Enemy *enemies, int max, void *params);
