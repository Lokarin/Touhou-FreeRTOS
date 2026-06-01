#pragma once

#include "input.h"
#include "renderer.h"
#include "bullet.h"
#include "sprites.h"


#define MAX_PLAYER_BULLETS 64
#define SHOOT_COOLDOWN 0.04f

typedef struct
{
    float x;
    float y;
    float speed;
    int w;
    int h;
    float hitbox_r;
    float vx;
    Bullet bullets[MAX_PLAYER_BULLETS];
    float shoot_timer;
} Player;

void player_init(Player *player);
void player_shoot(Player *player);
void player_update(Player *player, Input_State *input, float delta);
void player_draw(Player *player);
