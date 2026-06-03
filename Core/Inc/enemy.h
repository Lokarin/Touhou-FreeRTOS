#pragma once

#include "common.h"
#include "bullet.h"
#include "player.h"
#include "sprites.h"

#define MAX_ENEMIES 8

typedef enum
{
    ENEMY_WINDMILL,
    ENEMY_FAIRY,
    ENEMY_SOUL,
    ENEMY_BOSS,
} EnemyType;

typedef struct Enemy Enemy;

struct Enemy
{
    float x, y;
    int   w, h;
    float hitbox_r;
    int   hp;
    int   active;

    EnemyType type;

    float timer;
    int   state;
    float vel_x;
    float vel_y;

    float x_stop;
    float y_stop;

    void (*update)(Enemy *self, Player *player, float delta);
    void (*draw)  (Enemy *self);

    float anim_timer;
    int   anim_frame;

    const Sprite **frames;
    int            frame_count;
};

/* ── API pública ──────────────────────────────────────────── */

void enemies_init(Enemy *enemies, int max);

/* Retorna o índice do slot usado, ou -1 se array cheio */
int enemy_spawn(Enemy *enemies, int max,
                float x, float y,
                float vel_x, float vel_y,
                float x_stop, float y_stop,
                EnemyType type, int hp,
                int w, int h, float hitbox_r,
                const Sprite **frames, int frame_count,
                void (*update)(Enemy*, Player*, float),
                void (*draw)  (Enemy*));

void enemies_update(Enemy *enemies, int max, Player *player, float delta);
void enemies_draw  (Enemy *enemies, int max);
void enemy_draw_default(Enemy *self);
void enemy_check_bullet_hits(Enemy *enemy, Player *player);
