#pragma once

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

extern ScreenSystem *g_screen;

extern void screen_system_init(ScreenSystem *screen);

#define DRAW_MARGIN  (g_screen->draw_margin)
#define SCREEN_WIDTH (g_screen->screen_width)
#define SCREEN_HEIGHT (g_screen->screen_height)
#define PANEL_LEFT   (g_screen->panel_left)
#define PANEL_WIDTH  (g_screen->panel_width)
#define PANEL_UP     (g_screen->panel_up)
#define PANEL_HEIGHT (g_screen->panel_height)
#define PANEL_RIGHT  (g_screen->panel_right)
#define PANEL_DOWN   (g_screen->panel_down)
