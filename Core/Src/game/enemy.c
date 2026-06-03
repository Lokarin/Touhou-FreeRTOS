#include "enemy.h"
#include "collision.h"
#include "sprites.h"
#include <string.h>

void enemies_init(Enemy *enemies, int max)
{
    memset(enemies, 0, sizeof(Enemy) * max);
}

int enemy_spawn(Enemy *enemies, int max,
                float x, float y,
                float vel_x, float vel_y,
                float x_stop, float y_stop,
                EnemyType type, int hp,
                int w, int h, float hitbox_r,
                const Sprite **frames, int frame_count,
                void (*update)(Enemy*, Player*, float),
                void (*draw)  (Enemy*))
{
    for (int i = 0; i < max; i++)
    {
        if (!enemies[i].active)
        {
            enemies[i].x           = x;
            enemies[i].y           = y;
            enemies[i].vel_x       = vel_x;
            enemies[i].vel_y       = vel_y;
            enemies[i].x_stop      = x_stop;
            enemies[i].y_stop      = y_stop;
            enemies[i].w           = w;
            enemies[i].h           = h;
            enemies[i].hitbox_r    = hitbox_r;
            enemies[i].hp          = hp;
            enemies[i].type        = type;
            enemies[i].timer       = 0.0f;
            enemies[i].state       = 0;
            enemies[i].anim_timer  = 0.0f;
            enemies[i].anim_frame  = 0;
            enemies[i].frames      = frames;
            enemies[i].frame_count = frame_count;
            enemies[i].update      = update;
            enemies[i].draw        = draw;
            enemies[i].active      = 1;
            return i;  // retorna o índice real
        }
    }
    return -1;
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

            break;
        }
    }
}

void enemy_draw_default(Enemy *self)
{
    if (!self->frames || self->frame_count == 0) return;

    int frame = self->anim_frame % self->frame_count;
    const Sprite *spr = self->frames[frame];

    sprite_draw(spr,
                self->x - spr->width  / 2.0f,
                self->y - spr->height / 2.0f,
                0);
}

void enemies_update(Enemy *enemies, int max, Player *player, float delta)
{
    for (int i = 0; i < max; i++)
    {
        if (!enemies[i].active)
            continue;

        if (enemies[i].update)
            enemies[i].update(&enemies[i], player, delta);

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
