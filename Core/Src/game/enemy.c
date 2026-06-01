#include "enemy.h"
#include "collision.h"
#include "renderer.h"
#include <string.h>

void enemies_init(Enemy *enemies, int max)
{
    memset(enemies, 0, sizeof(Enemy) * max);
}

int enemy_spawn(Enemy *enemies, int max,
                float x, float y,
                EnemyType type, int hp,
                int w, int h, float hitbox_r,
                void (*update)(Enemy*, Player*, float),
                void (*draw)  (Enemy*))
{
    for (int i = 0; i < max; i++)
    {
        if (!enemies[i].active)
        {
            enemies[i].x        = x;
            enemies[i].y        = y;
            enemies[i].w        = w;
            enemies[i].h        = h;
            enemies[i].hitbox_r = hitbox_r;
            enemies[i].hp       = hp;
            enemies[i].type     = type;
            enemies[i].timer    = 0.0f;
            enemies[i].state    = 0;
            enemies[i].vel_x    = 0.0f;
            enemies[i].vel_y    = 0.0f;
            enemies[i].update   = update;
            enemies[i].draw     = draw;
            enemies[i].active   = 1;
            return 1;
        }
    }
    return 0;
}

void enemy_check_bullet_hits(Enemy *enemy, Player *player)
{
    Circle enemy_circle = {
        .x = enemy->x,
        .y = enemy->y,
        .r = enemy->hitbox_r
    };

    for (int b = 0; b < MAX_PLAYER_BULLETS; b++)
    {
        if (!player->bullets[b].active)
            continue;

        Circle bullet_circle = {
            .x = player->bullets[b].def.hitbox.x,
            .y = player->bullets[b].def.hitbox.y,
            .r = player->bullets[b].def.hitbox.r
        };

        if (circles_overlap(enemy_circle, bullet_circle))
        {
            player->bullets[b].active = 0;
            enemy->hp--;

            if (enemy->hp <= 0)
                enemy->active = 0;

            break; // um bullet por frame por inimigo
        }
    }
}

void enemies_update(Enemy *enemies, int max, Player *player, float delta)
{
    for (int i = 0; i < max; i++)
    {
        if (!enemies[i].active)
            continue;

        if (enemies[i].update)
            enemies[i].update(&enemies[i], player, delta);

        // Colisão com bullets é sempre igual, independente do tipo
        if (enemies[i].active)
            enemy_check_bullet_hits(&enemies[i], player);
    }
}

void enemies_draw(Enemy *enemies, int max)
{
    for (int i = 0; i < max; i++)
    {
        if (!enemies[i].active)
            continue;

        if (enemies[i].draw)
            enemies[i].draw(&enemies[i]);
    }
}
