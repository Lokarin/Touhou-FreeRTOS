#include "input.h"
#include "game_types.h"
#include "cmsis_os.h"

// Handle da queue declarado no main.c
extern osMessageQueueId_t qInputHandle;

void input_init(void)
{
    // Nada a inicializar - ADC/GPIO já inicializados no main antes do RTOS
}

void input_update(Input_State *state)
{
	InputState_t raw = { .joy_x = 2048, .joy_y = 2048, .sw = 1 };


    // Pega o dado mais recente da queue (timeout 0 = não bloqueia)
    osMessageQueueGet(qInputHandle, &raw, NULL, 0);

    state->left  = (raw.joy_x < JOY_THRESHOLD_LOW)  ? 1 : 0;
    state->right = (raw.joy_x > JOY_THRESHOLD_HIGH) ? 1 : 0;
    state->up    = (raw.joy_y < JOY_THRESHOLD_LOW)  ? 1 : 0;
    state->down  = (raw.joy_y > JOY_THRESHOLD_HIGH) ? 1 : 0;
    state->click = (raw.sw == 0)                     ? 1 : 0;
    state->pause = 0;
    state->quit  = 0;
}
