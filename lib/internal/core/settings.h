#pragma once

#include "core/game_state.h"

/** @brief Charge les paramètres depuis le fichier de configuration et les applique au contexte du jeu. Si le fichier de configuration n'existe pas, initialise les paramètres par défaut et crée le fichier avec init settings.
 * @param ctx Le contexte du jeu contenant les paramètres à charger 
 */
void load_settings(GameContext *ctx);

/** @brief Initialise les paramètres du jeu avec des valeurs par défaut
 * @param ctx Le contexte du jeu contenant les paramètres à initialiser
 */
void init_settings(GameContext *ctx);

/** @brief Enregistre les paramètres du jeu dans le fichier de configuration
 * @param ctx Le contexte du jeu contenant les paramètres à enregistrer
 */
void saveSettings(GameContext *ctx);
