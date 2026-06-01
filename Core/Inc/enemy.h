#pragma once

#include "common.h"
#include "bullet.h"
#include "player.h"

#define MAX_ENEMIES 8

typedef enum
{
    ENEMY_WINDMILL,
    ENEMY_FAIRY,
    ENEMY_SPIRIT,
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

    /* Estado interno genérico — cada tipo usa como quiser */
    float timer;   // ex: cooldown de tiro, tempo até virar
    int   state;   // ex: fase do movimento, modo de ataque
    float vel_x;   // velocidade atual (para inimigos que se movem)
    float vel_y;

    /* Comportamento injetado no spawn */
    void (*update)(Enemy *self, Player *player, float delta);
    void (*draw)  (Enemy *self);

    float anim_timer;   // tempo acumulado da animação
    int   anim_frame;   // frame atual
};

/* ── API pública ──────────────────────────────────────────── */

void enemies_init(Enemy *enemies, int max);

/* Spawna um inimigo com tipo, stats e callbacks.
   Retorna 1 se ok, 0 se array cheio.                        */
int enemy_spawn(Enemy *enemies, int max,
                float x, float y,
                EnemyType type, int hp,
                int w, int h, float hitbox_r,
                void (*update)(Enemy*, Player*, float),
                void (*draw)  (Enemy*));

/* Roda update + colisão com bullets do player para todos    */
void enemies_update(Enemy *enemies, int max, Player *player, float delta);

/* Chama draw de cada inimigo ativo                          */
void enemies_draw(Enemy *enemies, int max);

/* Exposta para reuso dentro dos callbacks de update         */
void enemy_check_bullet_hits(Enemy *enemy, Player *player);
