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

/**
 * @brief Initialise un InputSystem avec des keybinds par défaut.
 *
 * @param input InputSystem à initialiser.
 */
extern void input_initialize(InputSystem *input);

/**
 * @brief Met à jour l'état de toutes les touches/actions (appelé chaque frame).
 *
 * @param input InputSystem à mettre à jour.
 */
extern void input_update(InputSystem *input);

/**
 * @brief Indique si l'état key correspond à une pression sur cette frame.
 *
 * @param key État de l'action.
 * @return 1 si pressée cette frame, 0 sinon.
 */
extern int isPressed(KeyState key);

/**
 * @brief Indique si l'état key correspond à une touche maintenue.
 *
 * @param key État de l'action.
 * @return 1 si maintenue, 0 sinon.
 */
extern int isDown(KeyState key);

/**
 * @brief Indique si l'état key correspond à une relâche sur cette frame.
 *
 * @param key État de l'action.
 * @return 1 si relâchée cette frame, 0 sinon.
 */
extern int isReleased(KeyState key);

/**
 * @brief Change une keybind à une nouvelle valeur.
 *
 * @param key Pointeur vers le KeyboardKey à modifier (ex: &input->keybinds.shoot).
 * @param new_key Nouvelle touche à affecter.
 */
extern void input_change_keybind(KeyboardKey *key, KeyboardKey new_key);
