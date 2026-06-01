#pragma once

typedef enum
{
    STATE_MENU,
    STATE_PLAYING,
    STATE_GAME_OVER,
    STATE_WIN,
} GameState;

/* Estado global — lido por TaskGame e TaskDisplay */
extern GameState g_state;
