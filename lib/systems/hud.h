/**
 * @file hud.h
 * @brief Système d'affichage du HUD (Heads-Up Display)
 *
 * Affiche les informations du joueur sur la partie droite de l'écran :
 * - Nom du stage
 * - Score
 * - Vies restantes
 * - Bombes restantes
 *
 * Le HUD est dessiné à droite de la zone de jeu (PANEL_RIGHT)
 */

#pragma once

#include "core/game_state.h"

/**
 * @brief Dessine le HUD complet (score, vies, bombes, séparateur)
 * @param ctx Le contexte du jeu
 * @param stage_name Nom du stage affiché en haut du HUD
 */
extern void HUD_draw(GameContext *ctx, const char *stage_name);