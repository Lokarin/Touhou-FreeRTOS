#pragma once

#include "enemy.h"

/* Spawna um Windmill na posição (x, y).
   Retorna 1 se ok, 0 se array cheio.    */
int windmill_spawn(Enemy *enemies, int max, float x, float y);
