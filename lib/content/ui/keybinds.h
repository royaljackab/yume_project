#pragma once

#include "game_state.h"

void state_menu_keybinds_draw(GameContext *ctx);
void state_menu_keybinds_update(GameContext *ctx);
void state_menu_keybinds_init(GameContext *ctx);

void button_get_key(const char* target_name, KeyboardKey* key_target);