#pragma once

#include "bullet.h"
#include "player.h"

#define MAX_ENEMY_BULLETS 256

/* Pool global — declarado em main.c */
extern Bullet g_enemy_bullets[MAX_ENEMY_BULLETS];

/* Zera o pool inteiro */
void enemy_bullets_init(Bullet *pool, int max);

/* Move todos os bullets ativos e desativa os que saíram da tela */
void enemy_bullets_update(Bullet *pool, int max, float delta);

/* Checa colisão de todos os bullets ativos com o player.
   Desativa o bullet se acertar. Retorna 1 se o player foi acertado. */
int enemy_bullets_check_player(Bullet *pool, int max, Player *player);

/* Desenha todos os bullets ativos */
void enemy_bullets_draw(Bullet *pool, int max);
