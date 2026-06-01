#pragma once
#include <stdint.h>

/* Fundo ativo — pode ser trocado entre fases */
extern const uint16_t *g_background;

/* Define o fundo ativo */
void background_set(const uint16_t *bg);

/* Desenha uma região do fundo na tela — usado pelo dirty-rect */
void background_draw_region(int x, int y, int w, int h);
