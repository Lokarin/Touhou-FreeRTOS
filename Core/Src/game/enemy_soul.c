#include "enemy_soul.h"
#include "sprites.h"
#include <math.h>

#define SOUL_W           16
#define SOUL_H           16
#define SOUL_HITBOX_R    8.0f
#define SOUL_HP          12
#define SOUL_SPEED       60.0f
#define SOUL_ANIM_TIME   0.3f

static const Sprite *soul_frames[] =
{
    &soul_1,
    &soul_2
};

static void soul_update(Enemy *self, Player *player, float delta)
{
    (void)player;

    self->anim_timer += delta;
    if (self->anim_timer >= SOUL_ANIM_TIME)
    {
        self->anim_timer = 0.0f;
        self->anim_frame = !self->anim_frame;
    }

    self->timer += delta;

    self->y += self->vel_y * delta;

    self->x = self->x_stop +
              20.0f * sinf(self->y * 0.05f);

    if (self->y > SCREEN_H + SOUL_H)
    {
        self->active = 0;
    }
}

int soul_spawn(Enemy *enemies, int max, float x)
{
    return enemy_spawn(
        enemies,
        max,

        x,
        -SOUL_H,          // nasce acima da tela

        0.0f,
        SOUL_SPEED,       // movimento vertical

        x,
        0,

        ENEMY_SOUL,
        SOUL_HP,

        SOUL_W,
        SOUL_H,
        SOUL_HITBOX_R,

        soul_frames,
        2,

        soul_update,
        enemy_draw_default
    );
}

int soul_wave(Enemy *enemies, int max, void *params)
{
    SoulParams *p = (SoulParams *)params;
    return soul_spawn(enemies, max, p->x);
}
