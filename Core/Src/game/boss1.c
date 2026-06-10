#include "boss1.h"

#include "enemy_bullets.h"
#include "sprites.h"
#include "game_difficulty.h"

#include <math.h>

#define BOSS_W         32
#define BOSS_H         32
#define BOSS_HITBOX_R  15.0f

#define BOSS_X         (SCREEN_W / 2.0f)
#define BOSS_Y         40.0f

#define PHASE_1_HP     2000
#define PHASE_2_HP     1600
#define PHASE_3_HP     1300
#define PHASE_4_HP     1000
#define PHASE_5_HP     600
#define PHASE_LAST_BREATH 300

static const Sprite *boss_frames[] =
{
    &kurumi1
};

static void boss_anim(Enemy *self, float delta)
{
    self->anim_timer += delta;

    if (self->anim_timer > 0.25f)
    {
        self->anim_timer = 0.0f;
        self->anim_frame++;
    }
}

static void boss_move_entry(
    Enemy *self,
    float delta)
{
    float target_y = BOSS_Y;

    self->y += 25.0f * delta;

    if (self->y >= target_y)
    {
        self->y = target_y;

        self->state = BOSS_PHASE_1;

        self->phase_timer = 0.0f;
        self->shoot_timer = 0.0f;
        self->shoot_timer2 = 0.0f;
    }
}

static void boss_move_idle(Enemy *self, float delta)
{
    self->timer += delta;

    self->x =
        BOSS_X +
        20.0f * sinf(self->timer);

    self->y =
        BOSS_Y +
        2.0f * sinf(self->timer * 2.0f);
}


static void boss_move_center(Enemy *self, float delta)
{
    const float target_x = SCREEN_W / 2.0f;
    const float target_y = SCREEN_H / 2.0f;

    const float speed = 50.0f;

    float dx = target_x - self->x;
    float dy = target_y - self->y;

    float dist = sqrtf(dx * dx + dy * dy);

    if (dist < 1.0f)
    {
        self->x = target_x;
        self->y = target_y;
        return;
    }

    self->x += (dx / dist) * speed * delta;
    self->y += (dy / dist) * speed * delta;
}

static void boss_move_final(
    Enemy *self,
    float delta)
{
    const float target_x = SCREEN_W * 0.5f;
    const float target_y = BOSS_Y;

    float speed = 40.0f;

    float dx = target_x - self->x;
    float dy = target_y - self->y;

    float dist =
        sqrtf(dx*dx + dy*dy);

    if (dist < 1.0f)
    {
        self->x = target_x;
        self->y = target_y;
        return;
    }

    self->x += dx * speed * delta / dist;
    self->y += dy * speed * delta / dist;
}

static void pattern1(Enemy *self, Player *player, float delta)
{
    (void)player;

    self->shoot_timer += delta;

    if (self->shoot_timer < 0.8f)
        return;

    self->shoot_timer = 0.0f;

    int bullets =
        (int)(6 * difficulty_factor());

    for (int i = 0; i < bullets; i++)
    {
        float angle =
            (2.0f * M_PI * i) / bullets;

        BulletDef def;

        def.hitbox.x = self->x;
        def.hitbox.y = self->y;
        def.hitbox.r = 6.0f;

        def.sprite = &big_purple_bullet;

        def.dx = cosf(angle);
        def.dy = sinf(angle);

        def.speed = 120.0f;

        spawn_bullet(
            g_enemy_bullets,
            MAX_ENEMY_BULLETS,
            &def);
    }
}

static void spell1(Enemy *self, Player *player, float delta)
{
    (void)player;

    self->shoot_timer2 += delta;

    if (self->shoot_timer2 < 0.12f)
        return;

    self->shoot_timer2 = 0.0f;

    for (int i = 0; i < 2; i++)
    {
        float angle =
            self->shoot_angle +
            i * M_PI;

        BulletDef def;

        def.hitbox.x = self->x;
        def.hitbox.y = self->y;
        def.hitbox.r = 3.5f;

        def.sprite = &bullet_yellow_small;

        def.dx = cosf(angle);
        def.dy = sinf(angle);

        def.speed = 90.0f;

        spawn_bullet(
            g_enemy_bullets,
            MAX_ENEMY_BULLETS,
            &def);
    }

    self->shoot_angle -= 0.4f / difficulty_factor();
}

static void pattern2(
    Enemy *self,
    Player *player,
    float delta)
{
    self->shoot_timer += delta;

    if (self->shoot_timer < (0.5f / difficulty_factor()))
        return;

    self->shoot_timer = 0.0f;

    float dx = player->x - self->x;
    float dy = player->y - self->y;

    float len = sqrtf(dx * dx + dy * dy);

    if (len <= 0.0f)
        return;

    float base_angle = atan2f(dy, dx);

    BulletDef def;

    def.hitbox.x = self->x;
    def.hitbox.y = self->y;
    def.hitbox.r = 2.0f;

    def.sprite = &small_white_bullet;
    def.speed  = 100.0f;

    const float spread = 35.0f * (M_PI / 180.0f);

    float angles[] =
    {
        base_angle - spread,
        base_angle,
        base_angle + spread
    };

    for (int i = 0; i < 3; i++)
    {
        def.dx = cosf(angles[i]);
        def.dy = sinf(angles[i]);

        spawn_bullet(
            g_enemy_bullets,
            MAX_ENEMY_BULLETS,
            &def);
    }
}

static void spell2(Enemy *self, Player *player, float delta)
{
    (void)player;

    self->shoot_timer += delta;

    if (self->shoot_timer < (0.4f / (difficulty_factor() / 1.5f)))
        return;

    self->shoot_timer = 0.0f;

    int bullets = 12 * (difficulty_factor() / 1.5f);

    for (int i = 0; i < bullets; i++)
    {
        float angle =
            self->shoot_angle +
            (2.0f * M_PI * i) / bullets;

        BulletDef def;

        def.hitbox.x = self->x;
        def.hitbox.y = self->y;
        def.hitbox.r = 2.0f;

        def.sprite = &small_white_bullet;

        def.dx = cosf(angle);
        def.dy = sinf(angle);

        def.speed = 90.0f;

        spawn_bullet(
            g_enemy_bullets,
            MAX_ENEMY_BULLETS,
            &def);
    }

    self->shoot_angle += 0.2f;
}

static void final_spell(
    Enemy *self,
    Player *player,
    float delta)
{
    pattern2(self, player, delta);
    spell1(self, player, delta);
}

static void last_breath(
    Enemy *self,
    Player *player,
    float delta)
{
    (void)player;

    boss_move_center(self, delta);

    /* ──────────────────────────────── */
    /* Braços giratórios rápidos        */
    /* ──────────────────────────────── */

    self->shoot_timer += delta;

    if (self->shoot_timer >= 0.1f)
    {
        self->shoot_timer = 0.0f;

        for (int i = 0; i < 2; i++)
        {
            float angle =
                self->shoot_angle +
                i * M_PI;

            BulletDef def;

            def.hitbox.x = self->x;
            def.hitbox.y = self->y;
            def.hitbox.r = 2.0f;

            def.sprite = &small_white_bullet;

            def.dx = cosf(angle);
            def.dy = sinf(angle);

            def.speed = 80.0f;

            spawn_bullet(
                g_enemy_bullets,
                MAX_ENEMY_BULLETS,
                &def);
        }

        /* horário */
        self->shoot_angle += 0.11f * (difficulty_factor() / 1.5f);
    }

    /* ──────────────────────────────── */
    /* Cruz de balas grandes lenta      */
    /* ──────────────────────────────── */

    self->shoot_timer2 += delta;

    if (self->shoot_timer2 >= (2.7f / (difficulty_factor() / 1.5f)))
    {
        self->shoot_timer2 = 0.0f;

        BulletDef def;

        def.hitbox.x = self->x;
        def.hitbox.y = self->y;
        def.hitbox.r = 6.0f;

        def.sprite = &big_purple_bullet;

        def.speed = 45.0f;

        /* direita */
        def.dx =  1.0f;
        def.dy =  0.0f;
        spawn_bullet(
            g_enemy_bullets,
            MAX_ENEMY_BULLETS,
            &def);

        /* esquerda */
        def.dx = -1.0f;
        def.dy =  0.0f;
        spawn_bullet(
            g_enemy_bullets,
            MAX_ENEMY_BULLETS,
            &def);

        /* baixo */
        def.dx =  0.0f;
        def.dy =  1.0f;
        spawn_bullet(
            g_enemy_bullets,
            MAX_ENEMY_BULLETS,
            &def);

        /* cima */
        def.dx =  0.0f;
        def.dy = -1.0f;
        spawn_bullet(
            g_enemy_bullets,
            MAX_ENEMY_BULLETS,
            &def);

        /* ──────────────────────────────── */
        /* Diagonais médias                 */
        /* ──────────────────────────────── */

        BulletDef diag;

        diag.hitbox.x = self->x;
        diag.hitbox.y = self->y;
        diag.hitbox.r = 4.0f;

        diag.sprite = &bullet_yellow_small;

        diag.speed = 60.0f;

        const float d = 0.70710678f; // sqrt(2)/2

        /* baixo-direita */
        diag.dx =  d;
        diag.dy =  d;
        spawn_bullet(
            g_enemy_bullets,
            MAX_ENEMY_BULLETS,
            &diag);

        /* baixo-esquerda */
        diag.dx = -d;
        diag.dy =  d;
        spawn_bullet(
            g_enemy_bullets,
            MAX_ENEMY_BULLETS,
            &diag);

        /* cima-esquerda */
        diag.dx = -d;
        diag.dy = -d;
        spawn_bullet(
            g_enemy_bullets,
            MAX_ENEMY_BULLETS,
            &diag);

        /* cima-direita */
        diag.dx =  d;
        diag.dy = -d;
        spawn_bullet(
            g_enemy_bullets,
            MAX_ENEMY_BULLETS,
            &diag);
    }
}

static void boss1_update(
    Enemy *self,
    Player *player,
    float delta)
{
    boss_anim(self, delta);

    if (self->state == BOSS_ENTRY)
    {
        boss_move_entry(self, delta);

        if (self->y >= BOSS_Y)
        {
            self->state = BOSS_PHASE_1;

            self->phase_timer = 0.0f;
            self->shoot_timer = 0.0f;
            self->shoot_timer2 = 0.0f;
            self->shoot_angle = 0.0f;
        }

        return;
    }

    BossPhase target_phase;


    if (self->hp > PHASE_1_HP)
        target_phase = BOSS_PHASE_1;
    else if (self->hp > PHASE_2_HP)
        target_phase = BOSS_SPELL_1;
    else if (self->hp > PHASE_3_HP)
        target_phase = BOSS_PHASE_2;
    else if (self->hp > PHASE_4_HP)
        target_phase = BOSS_SPELL_2;
    else if (self->hp > PHASE_5_HP)
        target_phase = BOSS_FINAL;
    else
        target_phase = BOSS_LAST_BREATH;

    /* Entrou em uma nova fase */
    if (target_phase != self->state)
    {
        self->state = target_phase;

        self->phase_timer = 0.0f;

        self->shoot_timer  = 0.0f;
        self->shoot_timer2 = 0.0f;
        self->shoot_angle  = 0.0f;

        return;
    }

    self->phase_timer += delta;

    /* ─────────────────────────────── */
    /* Tempo de transição entre fases  */
    /* ─────────────────────────────── */

    if (self->phase_timer < 2.0f)
    {
        switch (self->state)
        {
            case BOSS_PHASE_1:
            case BOSS_SPELL_1:
            case BOSS_PHASE_2:
            case BOSS_SPELL_2:
                boss_move_idle(self, delta);
                break;

            case BOSS_FINAL:
                boss_move_final(self, delta);
                break;

            case BOSS_LAST_BREATH:
                boss_move_center(self, delta);
                break;
        }

        return;
    }

    /* ─────────────────────────────── */
    /* Fase ativa                      */
    /* ─────────────────────────────── */

    switch (self->state)
    {
        case BOSS_PHASE_1:
            boss_move_idle(self, delta);
            pattern1(self, player, delta);
            break;

        case BOSS_SPELL_1:
            boss_move_idle(self, delta);
            spell1(self, player, delta);
            break;

        case BOSS_PHASE_2:
            boss_move_idle(self, delta);
            pattern2(self, player, delta);
            break;

        case BOSS_SPELL_2:
            boss_move_idle(self, delta);
            spell2(self, player, delta);
            break;

        case BOSS_FINAL:
            boss_move_final(self, delta);
            final_spell(self, player, delta);
            break;

        case BOSS_LAST_BREATH:
            last_breath(self, player, delta);
            break;
    }
}

int boss1_spawn(Enemy *enemies, int max)
{
    return enemy_spawn(
        enemies,
        max,

        SCREEN_W / 2.0f,
        -40.0f,

        0.0f,
        20.0f,

        BOSS_X,
        BOSS_Y,

        ENEMY_BOSS,
        BOSS1_HP,

        BOSS_W,
        BOSS_H,
        BOSS_HITBOX_R,

        boss_frames,
        1,

        boss1_update,
        enemy_draw_default
    );
}

int boss1_wave(
    Enemy *enemies,
    int max,
    void *params)
{
    (void)params;

    return boss1_spawn(
        enemies,
        max);
}
