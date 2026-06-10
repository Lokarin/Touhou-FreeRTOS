#pragma once

#include "enemy.h"

#define BOSS1_HP 2300

typedef enum
{
    BOSS_ENTRY,

    BOSS_PHASE_1,
    BOSS_SPELL_1,
    BOSS_PHASE_2,
    BOSS_SPELL_2,
    BOSS_FINAL,
    BOSS_LAST_BREATH
} BossPhase;

int boss1_spawn(Enemy *enemies, int max);
int boss1_wave(Enemy *enemies, int max, void *params);
