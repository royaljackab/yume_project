#pragma once

#define DRAW_MARGIN 500
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PANEL_LEFT   (SCREEN_WIDTH / 64)
#define PANEL_WIDTH  (SCREEN_WIDTH / 2)
#define PANEL_UP     (SCREEN_HEIGHT / 45)
#define PANEL_HEIGHT (SCREEN_HEIGHT * 0.95)
#define PANEL_RIGHT  (PANEL_LEFT + PANEL_WIDTH)
#define PANEL_DOWN   (PANEL_UP + PANEL_HEIGHT)

typedef struct ScreenSystem {
    int screen_width;
    int screen_height;
    int draw_margin;
    int panel_left;
    int panel_width;
    int panel_up;
    int panel_height;
    int panel_right;
    int panel_down;
} ScreenSystem;

extern void screen_system_init(ScreenSystem *screen);
