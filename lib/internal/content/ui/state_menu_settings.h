/**
 * @file state_menu_settings.h
 * @brief Écran de configuration / réglages du menu
 *
 */

#pragma once
#include "core/game_state.h"

/**
 * @brief Applique volume_bgm et volume_sfx du contexte à tous les sons chargés.
 *        À appeler après load_settings() et à chaque modification de volume.
 * @param ctx contexte courant
 */
void audio_appliquer_volumes(GameContext *ctx);
