#include "level1.h"
#include "enemy_windmill.h"
#include "enemy_fairy.h"
#include "enemy_soul.h"
#include <math.h>


/* Parâmetros de cada spawn — static garante que os ponteiros
   void* na tabela continuam válidos durante toda a fase      */
static WindmillParams wm0 = { .y = 20.0f, .x_stop = 30.0f };
static WindmillParams wm1 = { .y = 20.0f, .x_stop = 98.0f };
static WindmillParams wm2 = { .y = 45.0f, .x_stop = 50.0f };
static WindmillParams wm3 = { .y = 45.0f, .x_stop = 78.0f };
static WindmillParams wm4 = { .y = 70.0f, .x_stop = 30.0f };
static WindmillParams wm5 = { .y = 70.0f, .x_stop = 98.0f };

static FairyParams fa1 = { .y = 15.0f, .phase = 0.0f, .pattern = FAIRY_CROSS };
static FairyParams fa2 = { .y = 30.0f, .phase = M_PI, .pattern = FAIRY_CROSS };
static FairyParams fa3 = { .y = 45.0f, .phase = 0.0f, .pattern = FAIRY_CROSS };
static FairyParams fa4 = { .y = 60.0f, .phase = M_PI, .pattern = FAIRY_CROSS };
static FairyParams fa5 = { .y = 35.0f, .phase = M_PI, .pattern = FAIRY_CIRCLE };
static FairyParams fa6 = { .y = 55.0f, .phase = 0.0f, .pattern = FAIRY_CIRCLE };

static SoulParams soul0 = { .x = 20.0f };
static SoulParams soul1 = { .x = 50.0f };
static SoulParams soul2 = { .x = 80.0f };

static const WaveEvent level1_events[] = {
    { 2.0f, windmill_wave_left,  &wm0 },
    { 2.1f, windmill_wave_right, &wm1 },
    { 2.2f, windmill_wave_left,  &wm2 },
    { 2.3f, windmill_wave_right, &wm3 },
    { 2.4f, windmill_wave_left,  &wm4 },
    { 2.5f, windmill_wave_right, &wm5 },
	{ 9.0f, fairy_wave_left,  &fa1 },
	{ 9.4f, fairy_wave_right,  &fa2 },
	{ 9.8f, fairy_wave_left,  &fa3 },
	{ 10.2f, fairy_wave_right,  &fa4 },
	{ 13.0f, soul_wave, &soul0 },
	{ 13.5f, soul_wave, &soul1 },
	{ 14.0f, soul_wave, &soul2 },
	{ 15.0f, fairy_wave_right,  &fa5 },
	{ 15.0f, fairy_wave_left,  &fa6 },
	{ 15.5f, fairy_wave_right,  &fa5 },
	{ 15.5f, fairy_wave_left,  &fa6 },
    { 30.0f, windmill_wave_right, &wm5 },
};

const Level level1 = {
    .events = level1_events,
    .count  = sizeof(level1_events) / sizeof(level1_events[0]),
};
