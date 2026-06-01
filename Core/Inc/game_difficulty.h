#pragma once

typedef enum
{
    DIFFICULTY_EASY,
    DIFFICULTY_MEDIUM,
    DIFFICULTY_LUNATIC,
} GameDifficulty;

extern GameDifficulty g_difficulty;

/* Estado interno do menu — compartilhado entre TaskGame e TaskDisplay */
extern int g_menu_state;   // 0 = tela start, 1 = seleção de dificuldade
extern int g_menu_cursor;  // 0 = Easy, 1 = Medium, 2 = Lunatic
