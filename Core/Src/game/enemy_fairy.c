#include "enemy_fairy.h"
#include "enemy_bullets.h"
#include "sprites.h"
#include "game_difficulty.h"
#include <math.h>

#define FAIRY_W              16
#define FAIRY_H              16
#define FAIRY_HITBOX_R       8.0f
#define FAIRY_HP             10
#define FAIRY_SPEED          60.0f
#define FAIRY_BULLET_SPD     40.0f
#define FAIRY_BULLET_R       2.0f
#define FAIRY_ANIM_TIME      0.3f
#define FAIRY_RING_BULLETS   3
#define FAIRY_WAVE_AMPLITUDE 8.0f
#define FAIRY_WAVE_FREQ      0.10f

static const Sprite *fairy_frames[] = { &fairy_1, &fairy_2 };

/* ── Lógica compartilhada ───────────────────────────────────── */

static void fairy_anim(Enemy *self, float delta)
{
    self->anim_timer += delta;
    if (self->anim_timer >= FAIRY_ANIM_TIME)
    {
        self->anim_timer = 0.0f;
        self->anim_frame = !self->anim_frame;
    }
}

static void fairy_shoot_ring(Enemy *self)
{
    int bullets = (int)(FAIRY_RING_BULLETS * difficulty_factor());

    for (int i = 0; i < bullets; i++)
    {
        float angle = self->y_stop + (2.0f * M_PI * i) / bullets;

        BulletDef def;
        def.hitbox.x = self->x;
        def.hitbox.y = self->y;
        def.hitbox.r = FAIRY_BULLET_R;
        def.sprite 	 = &small_white_bullet;
        def.dx       = cosf(angle);
        def.dy       = sinf(angle);
        def.speed    = FAIRY_BULLET_SPD * difficulty_factor();

        spawn_bullet(g_enemy_bullets, MAX_ENEMY_BULLETS, &def);
    }

    self->y_stop += M_PI / bullets;
}

/* ── Pattern: CROSS (atravessa a tela atirando em 4 pontos) ── */

static void fairy_update_cross(Enemy *self, Player *player, float delta)
{
    (void)player;

    fairy_anim(self, delta);

    self->x += self->vel_x * delta;
    self->y  = self->x_stop +
               FAIRY_WAVE_AMPLITUDE * sinf(self->x * FAIRY_WAVE_FREQ + self->timer);

    float p25 = SCREEN_W * 0.25f;
    float p50 = SCREEN_W * 0.50f;
    float p75 = SCREEN_W * 0.75f;

    if (self->vel_x > 0.0f)
    {
        if      (self->state == 0 && self->x >= p25) { fairy_shoot_ring(self); self->state = 1; }
        else if (self->state == 1 && self->x >= p50) { fairy_shoot_ring(self); self->state = 2; }
        else if (self->state == 2 && self->x >= p75) { fairy_shoot_ring(self); self->state = 3; }
        else if (self->state == 3 && self->x >= SCREEN_W) { fairy_shoot_ring(self); self->state = 4; }
    }
    else
    {
        if      (self->state == 0 && self->x <= p75) { fairy_shoot_ring(self); self->state = 1; }
        else if (self->state == 1 && self->x <= p50) { fairy_shoot_ring(self); self->state = 2; }
        else if (self->state == 2 && self->x <= p25) { fairy_shoot_ring(self); self->state = 3; }
        else if (self->state == 3 && self->x <= 0)   { fairy_shoot_ring(self); self->state = 4; }
    }

    if (self->x < -FAIRY_W || self->x > SCREEN_W + FAIRY_W)
        self->active = 0;
}

/* ── Pattern: CIRCLE (entra, faz um círculo, sai) ─────────── */

static void fairy_update_circle(Enemy *self, Player *player, float delta)
{
    (void)player;

    fairy_anim(self, delta);

    float radius = 20.0f;

    if (self->state == 0)
    {
        self->x += self->vel_x * delta;

        int reached = (self->vel_x > 0 && self->x >= self->x_stop) ||
                      (self->vel_x < 0 && self->x <= self->x_stop);

        if (reached)
        {
            self->x = self->x_stop;
            // ângulo inicial depende da direção de entrada
            self->timer = (self->vel_x < 0) ? 0.0f : (float)M_PI;
            self->state = 1;
        }
    }
    else if (self->state == 1)
    {
        float speed = 2.0f;
        float center_x = (self->vel_x < 0)
                         ? self->x_stop - radius
                         : self->x_stop + radius;

        self->timer += speed * delta;

        self->x = center_x + radius * cosf(self->timer);
        self->y = self->y_stop + radius * sinf(self->timer);

        // vel_y como contador de quarters — não conflita com anim_frame
        int quarter = (int)(self->timer / (M_PI / 2.0f));
        if (quarter != (int)self->vel_y && quarter <= 4)
        {
            fairy_shoot_ring(self);
            self->vel_y = (float)quarter;
        }

        if (self->timer >= 2.0f * M_PI + ((self->vel_x < 0) ? 0.0f : (float)M_PI))
            self->state = 2;
    }
    else if (self->state == 2)
    {
        // continua na mesma direção de entrada ← correção aqui
        self->x += self->vel_x * delta;

        if (self->x < -FAIRY_W || self->x > SCREEN_W + FAIRY_W)
            self->active = 0;
    }
}

/* ── Spawns ─────────────────────────────────────────────────── */

static void (*pattern_to_update[])(Enemy*, Player*, float) = {
    [FAIRY_CROSS]  = fairy_update_cross,
    [FAIRY_CIRCLE] = fairy_update_circle,
};

int fairy_spawn_left(Enemy *enemies, int max, FairyParams *p)
{
    float xs, ys;
    if (p->pattern == FAIRY_CROSS)
    {
        xs = p->y;    // centro da senoide
        ys = 0.0f;    // ângulo inicial dos tiros
    }
    else // FAIRY_CIRCLE
    {
        xs = SCREEN_W * 0.25f;  // para nesse x (1/4 da tela para spawn da esquerda)
        ys = p->y;              // centro y do círculo
    }

    int idx = enemy_spawn(
        enemies, max,
        -FAIRY_W, p->y,
        FAIRY_SPEED, 0.0f,
        xs, ys,
        ENEMY_FAIRY, FAIRY_HP,
        FAIRY_W, FAIRY_H, FAIRY_HITBOX_R,
        fairy_frames, 2,
        pattern_to_update[p->pattern],
        enemy_draw_default
    );

    if (idx >= 0)
        enemies[idx].timer = p->phase;

    return idx >= 0;
}

int fairy_spawn_right(Enemy *enemies, int max, FairyParams *p)
{
    float xs, ys;
    if (p->pattern == FAIRY_CROSS)
    {
        xs = p->y;
        ys = 0.0f;
    }
    else // FAIRY_CIRCLE
    {
        xs = SCREEN_W * 0.75f;  // para nesse x (3/4 da tela para spawn da direita)
        ys = p->y;
    }

    int idx = enemy_spawn(
        enemies, max,
        SCREEN_W + FAIRY_W, p->y,
        -FAIRY_SPEED, 0.0f,
        xs, ys,
        ENEMY_FAIRY, FAIRY_HP,
        FAIRY_W, FAIRY_H, FAIRY_HITBOX_R,
        fairy_frames, 2,
        pattern_to_update[p->pattern],
        enemy_draw_default
    );

    if (idx >= 0)
        enemies[idx].timer = p->phase;

    return idx >= 0;
}

/* ── Wave spawns ────────────────────────────────────────────── */

int fairy_wave_left(Enemy *enemies, int max, void *params)
{
    return fairy_spawn_left(enemies, max, (FairyParams *)params);
}

int fairy_wave_right(Enemy *enemies, int max, void *params)
{
    return fairy_spawn_right(enemies, max, (FairyParams *)params);
}
