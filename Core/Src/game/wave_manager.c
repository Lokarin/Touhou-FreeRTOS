#include "wave_manager.h"
#include "game_state.h"

static const Level *current_level = NULL;
static float        phase_time    = 0.0f;
static int          event_index   = 0;

void wave_manager_start(const Level *level)
{
    current_level = level;
    phase_time    = 0.0f;
    event_index   = 0;
}

void wave_manager_reset(void)
{
    phase_time  = 0.0f;
    event_index = 0;
}

int wave_manager_update(Enemy *enemies, int max, float delta)
{
    if (!current_level) return 0;

    phase_time += delta;

    // Dispara todos os eventos cujo tempo já chegou
    while (event_index < current_level->count)
    {
        const WaveEvent *ev = &current_level->events[event_index];

        if (phase_time >= ev->time)
        {
            ev->spawn_fn(enemies, max, ev->params);
            event_index++;
        }
        else
        {
            break; // eventos são ordenados por tempo, pode parar aqui
        }
    }

    // Retorna 1 quando todos os eventos foram disparados
    return (event_index >= current_level->count);
}
