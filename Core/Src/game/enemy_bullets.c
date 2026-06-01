#include "enemy_bullets.h"
#include "collision.h"
#include "renderer.h"
#include "common.h"
#include <string.h>

void enemy_bullets_init(Bullet *pool, int max)
{
    memset(pool, 0, sizeof(Bullet) * max);
}

void enemy_bullets_update(Bullet *pool, int max, float delta)
{
    for (int i = 0; i < max; i++)
    {
        if (!pool[i].active)
            continue;

        pool[i].def.hitbox.x += pool[i].def.dx * pool[i].def.speed * delta;
        pool[i].def.hitbox.y += pool[i].def.dy * pool[i].def.speed * delta;

        if (pool[i].def.hitbox.x < 0   || pool[i].def.hitbox.x > SCREEN_W ||
            pool[i].def.hitbox.y < 0   || pool[i].def.hitbox.y > SCREEN_H)
        {
            pool[i].active = 0;
        }
    }
}

int enemy_bullets_check_player(Bullet *pool, int max, Player *player)
{
    Circle player_circle = {
        .x = player->x,
        .y = player->y,
        .r = player->hitbox_r
    };

    for (int i = 0; i < max; i++)
    {
        if (!pool[i].active)
            continue;

        Circle bullet_circle = {
            .x = pool[i].def.hitbox.x,
            .y = pool[i].def.hitbox.y,
            .r = pool[i].def.hitbox.r
        };

        if (circles_overlap(player_circle, bullet_circle))
        {
            pool[i].active = 0;
            return 1; // player foi acertado
        }
    }

    return 0;
}

void enemy_bullets_draw(Bullet *pool, int max)
{
    for (int i = 0; i < max; i++)
    {
        if (!pool[i].active)
            continue;

        //renderer_draw_rect(pool[i].def.hitbox.x - 1, pool[i].def.hitbox.y - 1, 3, 3, 0xFF6600);
        sprite_draw(&bullet_green_small,
                    pool[i].def.hitbox.x - bullet_green_small.width  / 2,
                    pool[i].def.hitbox.y - bullet_green_small.height / 2,
                    0);
    }
}
