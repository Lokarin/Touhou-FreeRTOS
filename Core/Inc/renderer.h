#pragma once

void renderer_init(void);
void renderer_clear(void);
void renderer_present(void);
void renderer_shutdown(void);

void renderer_draw_rect(int x, int y, int w, int h, unsigned int color);
