#pragma once

#include "enemy.h"

typedef enum
{
    FAIRY_CROSS,
    FAIRY_CIRCLE,
} FairyPatterns;

typedef struct
{
    float         y;
    float         phase;
    FairyPatterns pattern;
} FairyParams;

int fairy_spawn_left (Enemy *enemies, int max, FairyParams *p);
int fairy_spawn_right(Enemy *enemies, int max, FairyParams *p);

int fairy_wave_left (Enemy *enemies, int max, void *params);
int fairy_wave_right(Enemy *enemies, int max, void *params);
