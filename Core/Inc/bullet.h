#pragma once

#include "common.h"
#include "sprites.h"

typedef struct {
    Circle          hitbox;
    float           dx, dy;
    float           speed;
    const Sprite   *sprite;  // NULL = usa sprite padrão do pool
} BulletDef;

typedef struct
{
    BulletDef def;
    int active;
} Bullet;

int  spawn_bullet  (Bullet *bullets, int max_bullets, BulletDef *def);
void update_bullets(Bullet *bullets, int max_bullets, float delta);
void draw_bullets  (Bullet *bullets, int max_bullets, const Sprite *default_sprite);
