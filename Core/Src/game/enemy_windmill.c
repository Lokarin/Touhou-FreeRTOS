#include "enemy_windmill.h"
#include "enemy_bullets.h"
#include "sprites.h"
#include "game_difficulty.h"
#include <math.h>

#define WINDMILL_W          20
#define WINDMILL_H          20
#define WINDMILL_HITBOX_R   4.0f
#define WINDMILL_HP         10
#define WINDMILL_IN_SPEED   40.0f
#define WINDMILL_SHOOT_TIME 4.0f
#define WINDMILL_SHOOT_CD   2.0f
#define WINDMILL_BULLET_SPD 60.0f
#define WINDMILL_BULLET_R   2.0f
#define WINDMILL_ANIM_TIME  0.2f

static const Sprite *windmill_frames[] = { &windmill_1, &windmill_2 };

static void windmill_update(Enemy *self, Player *player, float delta)
{
    self->anim_timer += delta;
    if (self->anim_timer >= WINDMILL_ANIM_TIME)
    {
        self->anim_timer = 0.0f;
        self->anim_frame = !self->anim_frame;
    }

    if (self->state == 0)
    {
        self->x += self->vel_x * delta;

        int reached = (self->vel_x > 0 && self->x >= self->x_stop) ||
                      (self->vel_x < 0 && self->x <= self->x_stop);

        if (reached)
        {
            self->x     = self->x_stop;
            self->timer = WINDMILL_SHOOT_TIME;
            self->vel_y = WINDMILL_SHOOT_CD / difficulty_factor();
            self->state = 1;
        }
    }
    else if (self->state == 1)
    {
        self->timer -= delta;
        self->vel_y -= delta;

        if (self->vel_y <= 0.0f)
        {
            self->vel_y = WINDMILL_SHOOT_CD / difficulty_factor();

            float dx  = player->x - self->x;
            float dy  = player->y - self->y;
            float len = sqrtf(dx*dx + dy*dy);

            if (len > 0.0f)
            {
                BulletDef def;
                def.hitbox.x = self->x;
                def.hitbox.y = self->y;
                def.hitbox.r = WINDMILL_BULLET_R;
                def.sprite = &bullet_green_small;
                def.dx       = dx / len;
                def.dy       = dy / len;
                def.speed    = WINDMILL_BULLET_SPD * difficulty_factor();
                spawn_bullet(g_enemy_bullets, MAX_ENEMY_BULLETS, &def);
            }
        }

        if (self->timer <= 0.0f)
            self->state = 2;
    }
    else if (self->state == 2)
    {
        self->x -= self->vel_x * delta;

        if (self->x < -WINDMILL_W || self->x > SCREEN_W + WINDMILL_W)
            self->active = 0;
    }
}

int windmill_spawn_left(Enemy *enemies, int max, float y, float x_stop)
{
    return enemy_spawn(enemies, max,
                       -WINDMILL_W, y,
                       WINDMILL_IN_SPEED, 0.0f,
                       x_stop, y,
                       ENEMY_WINDMILL, WINDMILL_HP,
                       WINDMILL_W, WINDMILL_H, WINDMILL_HITBOX_R,
                       windmill_frames, 2,
                       windmill_update,
                       enemy_draw_default) >= 0;
}

int windmill_spawn_right(Enemy *enemies, int max, float y, float x_stop)
{
    return enemy_spawn(enemies, max,
                       SCREEN_W + WINDMILL_W, y,
                       -WINDMILL_IN_SPEED, 0.0f,
                       x_stop, y,
                       ENEMY_WINDMILL, WINDMILL_HP,
                       WINDMILL_W, WINDMILL_H, WINDMILL_HITBOX_R,
                       windmill_frames, 2,
                       windmill_update,
                       enemy_draw_default) >= 0;
}

int windmill_wave_left(Enemy *enemies, int max, void *params)
{
    WindmillParams *p = (WindmillParams *)params;
    return windmill_spawn_left(enemies, max, p->y, p->x_stop);
}

int windmill_wave_right(Enemy *enemies, int max, void *params)
{
    WindmillParams *p = (WindmillParams *)params;
    return windmill_spawn_right(enemies, max, p->y, p->x_stop);
}
