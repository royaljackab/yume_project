/**
 * @file screen.c
 * @brief Initialisation du système d'écran
 * @author @amyelalem
 */

#include "lib/systems/screen.h"

void screen_system_init(ScreenSystem *screen) {
    screen->screen_width  = SCREEN_WIDTH;
    screen->screen_height = SCREEN_HEIGHT;
    screen->draw_margin   = DRAW_MARGIN;
    screen->panel_left    = PANEL_LEFT;
    screen->panel_width   = PANEL_WIDTH;
    screen->panel_up      = PANEL_UP;
    screen->panel_height  = PANEL_HEIGHT;
    screen->panel_right   = PANEL_RIGHT;
    screen->panel_down    = PANEL_DOWN;
}