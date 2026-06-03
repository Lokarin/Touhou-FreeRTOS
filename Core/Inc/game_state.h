#pragma once

#include "player.h"
#include "enemy.h"
#include "enemy_bullets.h"

typedef enum
{
    STATE_MENU,
    STATE_PLAYING,
    STATE_GAME_OVER,
    STATE_WIN,
} GameState;

extern GameState g_state;

extern Player g_player;
extern Enemy  g_enemies[];
extern Bullet g_enemy_bullets[];

void game_reset(void);
