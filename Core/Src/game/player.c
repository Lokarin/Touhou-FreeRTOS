#include "player.h"

void player_init(Player *player)
{
    player->x = 64.0f;
    player->y = 120.0f;
    player->speed = 80.0f;
    player->w = 10;
    player->h = 10;
    player->hitbox_r = 2.0f;
    player->shoot_timer = 0.0f;
}

void player_update(Player *player, Input_State *input, float delta)
{
    player->x += (input->right - input->left) * player->speed * delta;
    player->y += (input->down  - input->up)   * player->speed * delta;
    player->vx = (input->right - input->left) * player->speed;

    float half_w = player->w / 2.0f;
    float half_h = player->h / 2.0f;

    if (player->x - half_w < 0)
    {
        player->x = half_w;
    }

    if (player->x + half_w > 128)
    {
        player->x = 128 - half_w;
    }

    if (player->y - half_h < 0)
    {
        player->y = half_h;
    }

    if (player->y + half_h > 160)
    {
        player->y = 160 - half_h;
    }

    player->shoot_timer -= delta;

    if (player->shoot_timer <= 0.0f)
    {
        player_shoot(player);
        player->shoot_timer = SHOOT_COOLDOWN;
    }

    update_bullets(player->bullets, MAX_PLAYER_BULLETS, delta);
}

void player_shoot(Player *player)
{
    BulletDef def;
    def.speed    = 512.0f;
    def.hitbox.r = 3.0f;
    def.sprite = &player_bullet;

    // Fileira esquerda — levemente angulada para a esquerda
    def.hitbox.x = player->x - 5.0f;
    def.hitbox.y = player->y;
    def.dx = -0.12f;  // seno de ~10 graus
    def.dy = -0.99f;  // cosseno de ~10 graus
    spawn_bullet(player->bullets, MAX_PLAYER_BULLETS, &def);

    // Fileira do meio — direto para cima
    def.hitbox.x = player->x;
    def.hitbox.y = player->y;
    def.dx =  0.0f;
    def.dy = -1.0f;
    spawn_bullet(player->bullets, MAX_PLAYER_BULLETS, &def);

    // Fileira direita — levemente angulada para a direita
    def.hitbox.x = player->x + 5.0f;
    def.hitbox.y = player->y;
    def.dx =  0.12f;
    def.dy = -0.99f;
    spawn_bullet(player->bullets, MAX_PLAYER_BULLETS, &def);
}

//void player_draw(Player *player)
//{
//    draw_bullets(player->bullets, MAX_PLAYER_BULLETS);

//    renderer_draw_rect(player->x - player->w/2.0f, player->y - player->h/2.0f, player->w, player->h, 0xFF0000);
//}

void player_draw(Player *player)
{
	draw_bullets(player->bullets, MAX_PLAYER_BULLETS, &player_bullet);

    // escolhe sprite baseado no movimento
    if (player->vx > 0)
        sprite_draw(&player_movement, player->x - 6, player->y - 10, 1);
    else if (player->vx < 0)
        sprite_draw(&player_movement, player->x - 6, player->y - 10, 0);
    else
        sprite_draw(&player_static,   player->x - 6, player->y - 10, 0);
}
