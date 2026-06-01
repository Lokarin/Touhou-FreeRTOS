#include "enemy_windmill.h"
#include "enemy_bullets.h"
#include "renderer.h"
#include "sprites.h"
#include <math.h>

#define WINDMILL_W          20
#define WINDMILL_H          20
#define WINDMILL_HITBOX_R   4.0f
#define WINDMILL_HP         10
#define WINDMILL_IN_SPEED   40.0f
#define WINDMILL_SHOOT_CD   0.5f   // atira a cada 2 segundos
#define WINDMILL_BULLET_SPD 100.0f
#define WINDMILL_BULLET_R   2.0f
#define WINDMILL_ANIM_TIME  0.2f

/* ── Callbacks ──────────────────────────────────────────────── */

static void windmill_update(Enemy *self, Player *player, float delta)
{
	self->anim_timer += delta;
	if (self->anim_timer >= WINDMILL_ANIM_TIME)
	{
	    self->anim_timer = 0.0f;
	    self->anim_frame = !self->anim_frame;  // alterna entre 0 e 1
	}

    if (self->state == 0) // entrando
    {
        self->x += WINDMILL_IN_SPEED * delta;

        if (self->x >= 30.f)
        {
            self->x     = 30.f;
            self->timer = WINDMILL_SHOOT_CD; // começa o cooldown ao parar
            self->state = 1;
        }
    }
    else if (self->state == 1) // parado, atirando
    {
        self->timer -= delta;

        if (self->timer <= 0.0f)
        {
            self->timer = WINDMILL_SHOOT_CD;

            // Calcula direção até o player
            float dx = player->x - self->x;
            float dy = player->y - self->y;
            float len = sqrtf(dx*dx + dy*dy);

            if (len > 0.0f)
            {
                dx /= len; // normaliza
                dy /= len;

                BulletDef def;
                def.hitbox.x = self->x;
                def.hitbox.y = self->y;
                def.hitbox.r = WINDMILL_BULLET_R;
                def.dx       = dx;
                def.dy       = dy;
                def.speed    = WINDMILL_BULLET_SPD;

                spawn_bullet(g_enemy_bullets, MAX_ENEMY_BULLETS, &def);
            }
        }
    }
}

//static void windmill_draw(Enemy *self)
//{
//    renderer_draw_rect(
//        self->x - self->w / 2.0f,
//        self->y - self->h / 2.0f,
//        self->w, self->h,
//        0xFFFF00
//    );
//}

static void windmill_draw(Enemy *self)
{
    const Sprite *frame = (self->anim_frame == 0) ? &windmill_1 : &windmill_2;
    sprite_draw(frame, self->x - self->w / 2.0f, self->y - self->h / 2.0f, 0);
}

/* ── Spawn helper ───────────────────────────────────────────── */

int windmill_spawn(Enemy *enemies, int max, float x, float y)
{
    return enemy_spawn(enemies, max,
                       x, y,
                       ENEMY_WINDMILL, WINDMILL_HP,
                       WINDMILL_W, WINDMILL_H, WINDMILL_HITBOX_R,
                       windmill_update,
                       windmill_draw);
}
