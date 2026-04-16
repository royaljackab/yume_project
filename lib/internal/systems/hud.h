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
 * 
 * @author @amyelalem
 */

#pragma once

#include "core/game_state.h"

/** @brief Dessine le fond du HUD
 */
extern void HUD_draw_background(void);

/**
 * @brief Dessine le HUD
 * @param ctx Le contexte du jeu
 * @param stage_name Nom du stage affiché en haut du HUD
 */
extern void HUD_draw_foreground(GameContext *ctx, const char *stage_name);

// -- Variables animations spellcard
extern char hud_spell_name[128];
extern float hud_spell_x;
extern float hud_spell_y;
extern int hud_spell_alpha;
extern float hud_spell_scale;


/** @brief Efface la carte de la spellcard du HUD
 */
void HUD_clear_spellcard(void);