/**
 * @file lib/core/input.h
 * @brief Système d'entrée : keybinds et états des actions joueur.
 *
 * Fournit les structures et fonctions pour gérer les keybinds
 * et l'état des touches/actions (pressé / maintenu / relâché).
 */
#pragma once

#include <raylib.h>


/**
 * @brief Configuration des keybinds, modifiable dans les settings.
 *
 * @note Garder les mêmes noms de champs pour `InputConfig` et `InputSystem`.
 */
typedef struct InputConfig {
  // NOTE: gardez les memes noms de key pour config et State !
  KeyboardKey up, left, right, down;
  KeyboardKey shoot;
  KeyboardKey bomb;
  KeyboardKey focus;
  KeyboardKey skip;
  KeyboardKey pause;
  KeyboardKey validate;
} InputConfig;

/**
 * @brief État d'une touche/action (remplace les appels à IsKeyDown/IsKeyPressed/IsKeyReleased).
 */
typedef struct KeyState {
  int isPressed;
  int isDown;
  int isReleased;
} KeyState;

/**
 * @brief Système d'entrée complet contenant keybinds et états courants des touches.
 *
 * Contient une InputConfig (keybinds) et un KeyState par action.
 */
typedef struct InputSystem {
  // NOTE: gardez les memes noms de key pour config et State !
  InputConfig keybinds;

  KeyState up, left, right, down;
  KeyState shoot;
  KeyState bomb;
  KeyState focus;
  KeyState skip;
  KeyState pause;
  KeyState validate;
} InputSystem;

/* Fonctions */

/** @brief Initialise le système d'entrée
 * @param input Le système d'entrée à initialiser
 */
extern void input_initialize(InputSystem *input);

/** @brief Met à jour le système d'entrée en fonction de l'état actuel des touches
 * @param input Le système d'entrée à mettre à jour
 */
extern void input_update(InputSystem *input);

/** @brief Vérifie si une touche est actuellement pressée
 * @param key L'état de la touche à vérifier
 * @return 1 si la touche est pressée, 0 sinon
 */
extern int isPressed(KeyState key);

/** @brief Vérifie si une touche est actuellement enfoncée
 * @param key L'état de la touche à vérifier
 * @return 1 si la touche est enfoncée, 0 sinon
 */
extern int isDown(KeyState key);

/** @brief Vérifie si une touche vient d'être relâchée
 * @param key L'état de la touche à vérifier
 * @return 1 si la touche vient d'être relâchée, 0 sinon
 */
extern int isReleased(KeyState key);

/** @brief Modifie la touche associée à une action
 * @param key La touche à modifier
 * @param new_key La nouvelle touche
 */
extern void input_change_keybind(KeyboardKey *key, KeyboardKey new_key);
