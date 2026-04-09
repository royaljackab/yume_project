#pragma once


#include "game_state.h"
#include <stdbool.h>

void moonlight_bg_init(void);
void moonlight_bg_update(int frames);
void moonlight_bg_draw(GameContext *ctx);
void moonlight_bg_set_mode(bool to_spellcard);