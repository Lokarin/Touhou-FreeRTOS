#pragma once

#include "enemy.h"

/* Spawns diretos — usados pelo game_reset ou testes */
int windmill_spawn_left (Enemy *enemies, int max, float y, float x_stop);
int windmill_spawn_right(Enemy *enemies, int max, float y, float x_stop);

/* Params para uso em WaveEvent */
typedef struct {
    float y;
    float x_stop;
} WindmillParams;

/* Spawns via wave manager — assinatura genérica com void* */
int windmill_wave_left (Enemy *enemies, int max, void *params);
int windmill_wave_right(Enemy *enemies, int max, void *params);
