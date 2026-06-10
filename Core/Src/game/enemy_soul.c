#include "enemy_soul.h"
#include "enemy_bullets.h"
#include "sprites.h"
#include "game_difficulty.h"
#include <math.h>

#define SOUL_W         16
#define SOUL_H         16
#define SOUL_HITBOX_R  7.0f
#define SOUL_HP        14
#define SOUL_SPEED     60.0f
#define SOUL_ANIM_TIME 0.3f
#define SOUL_DEATH_BULLETS 2
#define SOUL_SHOT_CD     0.15f
#define SOUL_BULLET_SPD  35.0f
#define SOUL_BULLET_R    2.0f

static const Sprite *soul_frames[] =
{
    &soul_1,
    &soul_2
};

static void soul_anim(Enemy *self, float delta)
{
    self->anim_timer += delta;

    if (self->anim_timer >= SOUL_ANIM_TIME)
    {
        self->anim_timer = 0.0f;
        self->anim_frame = !self->anim_frame;
    }
}

static void soul_shoot(Enemy *self, float delta)
{
    self->shoot_timer += delta;

    if (self->shoot_timer < SOUL_SHOT_CD)
        return;

    self->shoot_timer = 0.0f;

    BulletDef def;

    def.hitbox.x = self->x;
    def.hitbox.y = self->y;
    def.hitbox.r = SOUL_BULLET_R;

    def.sprite = &small_white_bullet;

    def.dx = cosf(self->shoot_angle);
    def.dy = sinf(self->shoot_angle);

    def.speed =
        SOUL_BULLET_SPD *
        difficulty_factor();

    spawn_bullet(
        g_enemy_bullets,
        MAX_ENEMY_BULLETS,
        &def);

    self->shoot_angle += 0.55f;
}

static void soul_update_straight(
    Enemy *self,
    Player *player,
    float delta)
{
    (void)player;

    soul_anim(self, delta);

    self->y += self->vel_y * delta;

    soul_shoot(self, delta);

    if (self->y > SCREEN_H + SOUL_H)
        self->active = 0;
}

static void soul_update_sine(
    Enemy *self,
    Player *player,
    float delta)
{
    (void)player;

    soul_anim(self, delta);

    self->y += self->vel_y * delta;

    self->x =
        self->x_stop +
        self->y_stop *
        sinf(
            self->y * self->vel_x +
            self->timer
        );

    soul_shoot(self, delta);

    if (self->y > SCREEN_H + SOUL_H)
        self->active = 0;
}

static void soul_update_helix(
    Enemy *self,
    Player *player,
    float delta)
{
    (void)player;

    soul_anim(self, delta);

    self->timer += self->vel_x * delta;

    // centro da hélice descendo
    self->y_stop += self->vel_y * delta;

    float radius_x = 10.0f;
    float radius_y = 20.0f;

    self->x =
        self->x_stop +
        radius_x * cosf(self->timer);

    self->y =
        self->y_stop +
        radius_y * sinf(self->timer);

    soul_shoot(self, delta);

    if (self->y > SCREEN_H + SOUL_H)
        self->active = 0;
}

static void soul_on_death(Enemy *self, Player *player)
{
    float dx  = player->x - self->x;
    float dy  = player->y - self->y;
    float len = sqrtf(dx*dx + dy*dy);
    if (len <= 0.0f) return;

    BulletDef def;
    def.hitbox.x = self->x;
    def.hitbox.y = self->y;
    def.hitbox.r = 2.0f;
    def.sprite   = &small_white_bullet;
    def.dx       = dx / len;
    def.dy       = dy / len;
    def.speed    = 30.0f * difficulty_factor();

    int bullets = (int)(SOUL_DEATH_BULLETS * difficulty_factor());
    float spread = M_PI / 3.0f; // 60°

    // Dispara um leque de 5 balas centrado na direção do player
    float base_angle = atan2f(dy, dx);
    for (int i = 0; i < bullets; i++)
    {
        float angle;

        if (bullets == 1)
        {
            angle = base_angle;
        }
        else
        {
            angle =
                base_angle
                - spread * 0.5f
                + spread * ((float)i / (bullets - 1));
        }

        def.dx = cosf(angle);
        def.dy = sinf(angle);

        spawn_bullet(
            g_enemy_bullets,
            MAX_ENEMY_BULLETS,
            &def);
    }
}

static void (* const pattern_to_update[])
(
    Enemy*,
    Player*,
    float
)
=
{
    [SOUL_STRAIGHT] = soul_update_straight,
    [SOUL_SINE]     = soul_update_sine,
    [SOUL_HELIX]   = soul_update_helix,
};

int soul_spawn(
    Enemy *enemies,
    int max,
    SoulParams *p)
{
    int idx =
        enemy_spawn(
            enemies,
            max,

            p->x,
            -SOUL_H,

            0.0f,
            SOUL_SPEED,

            p->x,
            p->amplitude,

            ENEMY_SOUL,
            SOUL_HP,

            SOUL_W,
            SOUL_H,
            SOUL_HITBOX_R,

            soul_frames,
            2,

            pattern_to_update[p->pattern],
            enemy_draw_default
        );

    if (idx >= 0)
    {
        enemies[idx].vel_x      = p->frequency;
        enemies[idx].timer      = p->phase;
        enemies[idx].on_death   = soul_on_death;
    }

    return idx;
}

int soul_wave(
    Enemy *enemies,
    int max,
    void *params)
{
    return soul_spawn(
        enemies,
        max,
        (SoulParams *)params
    );
}
