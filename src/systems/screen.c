/**
 * @file screen.c
 * @brief Système d'écran 
 * @author @amyelalem
 */

#include "systems/screen.h"
#include <stdlib.h>

ScreenSystem *g_screen = NULL;

void screen_system_init(ScreenSystem *screen) {
    screen->screen_width  = 1920;
    screen->screen_height = 1080;
    screen->draw_margin   = 500;
    screen->panel_left    = screen->screen_width / 64;
    screen->panel_width   = screen->screen_width / 1.8;
    screen->panel_up      = screen->screen_height / 45;
    screen->panel_height  = (int)(screen->screen_height * 0.95f);
    screen->panel_right   = screen->panel_left + screen->panel_width;
    screen->panel_down    = screen->panel_up + screen->panel_height;

    g_screen = screen;
}