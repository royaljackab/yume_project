/**
 * @file moonlight_bg.h
 * @brief Arrière-plan et effets pour le stage "moonlight"
 *
 * Gère l'initialisation, la mise à jour par frame, le rendu et le changement
 * de mode (spellcard ou non) de l'arrière-plan du stage Moonlight.
 */

#pragma once

#include "game_state.h"
#include <stdbool.h>

/**
 * @brief Initialise les ressources et l'état de l'arrière-plan Moonlight.
 */
void moonlight_bg_init(void);

/**
 * @brief Met à jour l'arrière-plan Moonlight.
 *
 * @param frames Nombre de frames écoulées depuis le dernier appel (généralement 1).
 */
void moonlight_bg_update(int frames);

/**
 * @brief Dessine l'arrière-plan du stage Moonlight.
 *
 * @param ctx Contexte de jeu contenant la pool et les systèmes nécessaires.
 */
void moonlight_bg_draw(GameContext *ctx);

/**
 * @brief Change le mode d'affichage de l'arrière-plan (vers/depuis spellcard).
 *
 * @param to_spellcard true pour activer le mode spellcard, false pour revenir au mode normal.
 */
void moonlight_bg_set_mode(bool to_spellcard);