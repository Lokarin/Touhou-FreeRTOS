#pragma once

typedef struct 
{
    int up, down, left, right;
    int click;
    int pause;
    int quit;
} Input_State;

void input_init(void);
void input_update(Input_State *state);
