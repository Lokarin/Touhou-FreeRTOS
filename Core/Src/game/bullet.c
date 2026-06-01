#include "bullet.h"
#include "sprites.h"
#include "renderer.h"

int spawn_bullet(Bullet *bullets, int max_bullets, BulletDef *def)
{
    for (int i = 0; i < max_bullets; i++)
    {
        if (!bullets[i].active)
        {
            bullets[i].def    = *def; 
            bullets[i].active = 1;
            return 1;
        }
    }
    return 0;
}

void update_bullets(Bullet *bullets, int max_bullets, float delta)
{
    for (int i = 0; i < max_bullets; i++)
    {
        if (bullets[i].active)
        {
            bullets[i].def.hitbox.x += bullets[i].def.dx * bullets[i].def.speed * delta;
            bullets[i].def.hitbox.y += bullets[i].def.dy * bullets[i].def.speed * delta;

            if (bullets[i].def.hitbox.y < 0 || bullets[i].def.hitbox.y > 160 ||
                bullets[i].def.hitbox.x < 0 || bullets[i].def.hitbox.x > 128)
            {
                bullets[i].active = 0;
            }
        }
    }
}

void draw_bullets(Bullet *bullets, int max_bullets)
{
    for (int i = 0; i < max_bullets; i++)
    {
        if (bullets[i].active)
        {
            //renderer_draw_rect(bullets[i].def.hitbox.x, bullets[i].def.hitbox.y, 2, 4, 0xFFFFFF);
        	sprite_draw(&player_bullet,
        	            bullets[i].def.hitbox.x - player_bullet.width  / 2.0f,
        	            bullets[i].def.hitbox.y - player_bullet.height / 2.0f,
        	            0);
        }
    }
}
