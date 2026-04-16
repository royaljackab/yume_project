/**
 * @file moonlight.h
 * @brief fonctions décrivant le game state moonlight
 * 
 */
#pragma once
#include "core/game_state.h"

extern void state_moonlight_update(GameContext *ctx);
extern void state_moonlight_draw(GameContext *ctx);
extern void state_moonlight_init(GameContext *ctx);
extern void state_moonlight_cleanup(GameContext *ctx);
