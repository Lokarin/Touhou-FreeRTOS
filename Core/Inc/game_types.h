#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <stdint.h>

// Leitura bruta do hardware - produzida pela TaskInput
typedef struct {
    uint16_t joy_x;   // 0-4095
    uint16_t joy_y;   // 0-4095
    uint8_t  sw;      // 0 = pressionado, 1 = solto
} InputState_t;

// Thresholds para conversão analógico -> digital
#define JOY_THRESHOLD_LOW  1500
#define JOY_THRESHOLD_HIGH 2500

#endif
