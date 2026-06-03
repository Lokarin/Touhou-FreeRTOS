#pragma once

#include "enemy.h"

/* Um evento na tabela de uma fase */
typedef struct {
    float time;                                    // segundos desde início da fase
    int (*spawn_fn)(Enemy*, int, void *params);    // função de spawn
    void *params;                                  // dados específicos do tipo
} WaveEvent;

/* Descrição completa de uma fase */
typedef struct {
    const WaveEvent *events;
    int              count;
} Level;

/* ── API pública ──────────────────────────────────────────── */

/* Inicia o wave manager com um nível */
void wave_manager_start(const Level *level);

/* Atualiza o wave manager — chama os spawns no tempo certo.
   Retorna 1 quando todos os eventos foram disparados.       */
int wave_manager_update(Enemy *enemies, int max, float delta);

/* Reseta o wave manager (chamado pelo game_reset) */
void wave_manager_reset(void);
