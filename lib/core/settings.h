/**
 * @file lib/core/settings.h
 * @brief Fonctions de chargement, initialisation et sauvegarde des settings.
 *
 */
#pragma once

#include "core/game_state.h"

/**
 * @brief Charge les paramètres depuis le fichier settings.txt si présent.
 *
 * @param ctx Contexte de jeu contenant les structures où stocker les settings.
 */
void load_settings(GameContext *ctx);

/**
 * @brief Initialise les paramètres par défaut lorsque aucun fichier n'existe.
 *
 * @param ctx Contexte de jeu où initialiser les settings par défaut.
 */
void init_settings(GameContext *ctx);

/**
 * @brief Sauvegarde les paramètres courants dans le fichier settings.txt.
 *
 * @param ctx Contexte de jeu contenant les paramètres à écrire.
 */
void saveSettings(GameContext *ctx);
