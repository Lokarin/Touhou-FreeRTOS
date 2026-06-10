#include "level1.h"
#include "enemy_windmill.h"
#include "enemy_fairy.h"
#include "enemy_soul.h"
#include "boss1.h"
#include <math.h>

/* ───────────────── Windmills ───────────────── */

static WindmillParams wm0 = { .y = 20.0f, .x_stop = 30.0f };
static WindmillParams wm1 = { .y = 20.0f, .x_stop = 98.0f };
static WindmillParams wm2 = { .y = 45.0f, .x_stop = 50.0f };
static WindmillParams wm3 = { .y = 45.0f, .x_stop = 78.0f };
static WindmillParams wm4 = { .y = 70.0f, .x_stop = 30.0f };
static WindmillParams wm5 = { .y = 70.0f, .x_stop = 98.0f };

/* ───────────────── Fairies ───────────────── */

static FairyParams fa1 = { .y = 20.0f, .phase = 0.0f, .pattern = FAIRY_CROSS };
static FairyParams fa2 = { .y = 35.0f, .phase = M_PI, .pattern = FAIRY_CROSS };
static FairyParams fa3 = { .y = 50.0f, .phase = 0.0f, .pattern = FAIRY_CROSS };
static FairyParams fa4 = { .y = 65.0f, .phase = M_PI, .pattern = FAIRY_CROSS };

static FairyParams fa5 = { .y = 35.0f, .phase = M_PI, .pattern = FAIRY_CIRCLE };
static FairyParams fa6 = { .y = 55.0f, .phase = 0.0f, .pattern = FAIRY_CIRCLE };

/* ───────────────── Souls ───────────────── */

static SoulParams s0 =
{
    .x = 20.0f,
    .amplitude = 0.0f,
    .frequency = 0.0f,
    .phase = 0.0f,
    .pattern = SOUL_STRAIGHT
};

static SoulParams s1 =
{
    .x = 50.0f,
    .amplitude = 20.0f,
    .frequency = 0.05f,
    .phase = 0.0f,
    .pattern = SOUL_SINE
};

static SoulParams s2 =
{
    .x = 80.0f,
    .amplitude = 15.0f,
    .frequency = 3.0f,
    .phase = 0.0f,
    .pattern = SOUL_HELIX
};

static SoulParams s3 =
{
    .x = 32.0f,
    .amplitude = 20.0f,
    .frequency = 0.05f,
    .phase = M_PI,
    .pattern = SOUL_SINE
};

static SoulParams s4 =
{
    .x = 96.0f,
    .amplitude = 20.0f,
    .frequency = 0.05f,
    .phase = 0.0f,
    .pattern = SOUL_SINE
};

/* ───────────────── Timeline ───────────────── */

static const WaveEvent level1_events[] =
{
		/* =======================================================
           0-15s ─ Introdução com Souls
           ======================================================= */

        {  2.0f, soul_wave, &s0 },
        {  4.0f, soul_wave, &s1 },
        {  6.0f, soul_wave, &s2 },

        { 10.0f, soul_wave, &s1 },
        { 10.0f, soul_wave, &s3 },

        { 13.0f, soul_wave, &s4 },

        /* =======================================================
           15-30s ─ Fairies densas
           ======================================================= */

        { 15.0f, fairy_wave_left,  &fa1 },
        { 15.0f, fairy_wave_right, &fa3 },

        { 15.6f, fairy_wave_left,  &fa2 },
        { 15.6f, fairy_wave_right, &fa4 },

        { 19.0f, fairy_wave_left,  &fa3 },
        { 19.2f, fairy_wave_right, &fa1 },
        { 19.6f, fairy_wave_right, &fa2 },
        { 19.8f, fairy_wave_left,  &fa4 },

        { 23.0f, fairy_wave_left,  &fa1 },
        { 23.0f, fairy_wave_left,  &fa3 },
        { 23.0f, fairy_wave_right, &fa2 },
        { 23.0f, fairy_wave_right, &fa4 },

        { 27.0f, fairy_wave_left,  &fa2 },
        { 27.2f, fairy_wave_right, &fa1 },
        { 27.4f, fairy_wave_left,  &fa4 },
        { 27.6f, fairy_wave_right, &fa3 },

        /* =======================================================
           30-45s ─ Windmill wave clássica
           ======================================================= */

        { 30.0f, windmill_wave_left,  &wm0 },
        { 30.1f, windmill_wave_right, &wm1 },
        { 30.2f, windmill_wave_left,  &wm2 },
        { 30.3f, windmill_wave_right, &wm3 },
        { 30.4f, windmill_wave_left,  &wm4 },
        { 30.5f, windmill_wave_right, &wm5 },

        { 32.0f, soul_wave, &s2 },
        { 32.0f, soul_wave, &s4 },

        { 35.0f, soul_wave, &s1 },
        { 35.0f, soul_wave, &s2 },

        { 40.0f, soul_wave, &s3 },
        { 40.0f, soul_wave, &s4 },

        /* =======================================================
           45-60s ─ Fairies circulares
           ======================================================= */

        { 45.0f, fairy_wave_left,  &fa5 },
        { 45.0f, fairy_wave_right, &fa6 },
        { 45.5f, fairy_wave_left,  &fa5 },
        { 45.5f, fairy_wave_right, &fa6 },
        { 46.0f, fairy_wave_left,  &fa5 },
        { 46.0f, fairy_wave_right, &fa6 },

        { 50.0f, fairy_wave_left,  &fa1 },
        { 50.0f, fairy_wave_left,  &fa3 },
        { 50.0f, fairy_wave_right, &fa2 },
        { 50.0f, fairy_wave_right, &fa4 },

        { 55.0f, fairy_wave_left,  &fa5 },
        { 55.0f, fairy_wave_right, &fa6 },
        { 55.5f, fairy_wave_left,  &fa5 },
        { 55.5f, fairy_wave_right, &fa6 },
        { 56.0f, fairy_wave_left,  &fa5 },
        { 56.0f, fairy_wave_right, &fa6 },

        /* =======================================================
           60-75s ─ Windmills persistentes
           ======================================================= */

        { 60.0f, windmill_wave_left,  &wm4 },
        { 60.0f, windmill_wave_right,  &wm2 },
        { 61.0f, soul_wave, &s2 },
        { 63.0f, windmill_wave_right, &wm5 },
        { 63.0f, windmill_wave_left, &wm1 },
        { 66.0f, windmill_wave_left,  &wm4 },
        { 66.0f, soul_wave, &s1 },
        { 66.0f, windmill_wave_right,  &wm0 },
        { 69.0f, windmill_wave_right, &wm5 },
        { 69.0f, soul_wave, &s2 },
        { 69.0f, windmill_wave_left, &wm4 },
        { 69.0f, soul_wave, &s3 },
        { 72.0f, windmill_wave_left,  &wm4 },
        { 72.0f, windmill_wave_right,  &wm1 },
        { 72.3f, soul_wave, &s2 },
        { 72.8f, soul_wave, &s1 },
        { 73.0f, soul_wave, &s3 },
        { 74.0f, soul_wave, &s2 },

        /* =======================================================
           75-90s ─ Clímax
           ======================================================= */

        { 75.0f, windmill_wave_left,  &wm0 },
        { 75.0f, soul_wave,           &s1 },

        { 77.0f, windmill_wave_right, &wm1 },
        { 77.0f, soul_wave,           &s2 },

        { 79.0f, fairy_wave_left,     &fa1 },
        { 79.0f, fairy_wave_left,     &fa2 },
        { 79.0f, fairy_wave_right,    &fa3 },
        { 79.0f, fairy_wave_right,    &fa4 },

        { 82.0f, windmill_wave_left,  &wm2 },
        { 82.0f, soul_wave,           &s3 },

        { 84.0f, windmill_wave_right, &wm3 },
        { 84.0f, soul_wave,           &s4 },

        { 86.0f, fairy_wave_left,     &fa5 },
        { 86.0f, fairy_wave_right,    &fa6 },

        { 88.0f, fairy_wave_left,     &fa1 },
        { 88.0f, fairy_wave_left,     &fa3 },
        { 88.0f, fairy_wave_right,    &fa2 },
        { 88.0f, fairy_wave_right,    &fa4 },

        { 89.0f, windmill_wave_left,  &wm4 },
        { 89.0f, windmill_wave_right, &wm5 },

        /* =======================================================
           90-100s ─ Clímax II
           ======================================================= */

        { 90.0f, windmill_wave_left,  &wm0 },
        { 90.0f, soul_wave,           &s1 },

        { 92.0f, windmill_wave_right, &wm1 },
        { 92.0f, soul_wave,           &s2 },

        { 94.0f, fairy_wave_left,     &fa1 },
        { 94.2f, fairy_wave_right,    &fa2 },
        { 94.4f, fairy_wave_left,     &fa3 },
        { 94.6f, fairy_wave_right,    &fa4 },

        { 97.0f, windmill_wave_left,  &wm2 },
        { 97.0f, soul_wave,           &s3 },

        { 99.0f, windmill_wave_right, &wm3 },
        { 99.0f, soul_wave,           &s4 },

        /* =======================================================
           100-108s ─ Fairy Storm
           ======================================================= */

        {100.0f, fairy_wave_left,  &fa1 },
        {100.2f, fairy_wave_right, &fa2 },
        {100.4f, fairy_wave_left,  &fa3 },
        {100.6f, fairy_wave_right, &fa4 },

        {103.0f, fairy_wave_left,  &fa5 },
        {103.0f, fairy_wave_right, &fa6 },

        {106.0f, fairy_wave_left,  &fa1 },
        {106.2f, fairy_wave_right, &fa2 },
        {106.4f, fairy_wave_left,  &fa3 },
        {106.6f, fairy_wave_right, &fa4 },

        /* =======================================================
           108-115s ─ Última pressão
           ======================================================= */

        {108.0f, windmill_wave_left,  &wm4 },
        {108.0f, soul_wave,           &s1 },

        {110.0f, windmill_wave_right, &wm5 },
        {110.0f, soul_wave,           &s2 },

        {112.0f, fairy_wave_left,     &fa5 },
        {112.0f, fairy_wave_right,    &fa6 },

        {113.0f, fairy_wave_left,     &fa1 },
        {113.2f, fairy_wave_right,    &fa2 },
        {113.4f, fairy_wave_left,     &fa3 },
        {113.6f, fairy_wave_right,    &fa4 },

        {114.0f, windmill_wave_left,  &wm2 },
        {114.0f, windmill_wave_right, &wm3 },

        {114.5f, soul_wave,           &s3 },
        {114.5f, soul_wave,           &s4 },

        /* =======================================================
           BOSS
           ======================================================= */
        { 120.0f, boss1_wave, NULL },

        {300.5f, soul_wave,           &s4 },
};

const Level level1 =
{
    .events = level1_events,
    .count  = sizeof(level1_events) / sizeof(level1_events[0]),
};
