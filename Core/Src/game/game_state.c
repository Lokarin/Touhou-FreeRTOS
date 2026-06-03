#include "game_state.h"
#include "enemy_windmill.h"
#include "wave_manager.h"
#include "level1.h"

void game_reset(void)
{
    player_init(&g_player);
    enemies_init(g_enemies, MAX_ENEMIES);
    enemy_bullets_init(g_enemy_bullets, MAX_ENEMY_BULLETS);
    wave_manager_start(&level1);
}
