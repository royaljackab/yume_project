/**
 * @file game_state.h
 * @brief Fonctions permettant de gêrer l'état du programme.
 */
#pragma once

#include "core/input.h"
#include "coroutine/cosched.h"
#include "ecs/pool.h"
#include "systems/button.h"
#include "systems/score.h"
#include <raylib.h>
#include "../systems/screen.h"

/**
 * @brief Les différents états du programme
 * On trouve les menus, le stage, les différentes pages de texte.
 * 
 */
typedef enum StateID {
  STATE_NONE,

  __BEGIN_MENU__,

  STATE_MENU_TITLE,
  STATE_MENU_SETTINGS,
  STATE_MENU_KEYBINDS,

  __END_MENU__,

  STATE_MOONLIGHT,
  STATE_GAME_OVER,
  STATE_VICTORY,
  STATE_TEST,
  STATE_EXTRA
} StateID;

typedef struct GameContext GameContext;

/**
 * @brief Un GameState représente un état du jeu.
 * Chaque GameState dispose de 4 fonctions, passée par pointeurs:
 * - init est appellée lorsque le programme bascule sur cet état
 * - update est appellée à chaque frame
 * - draw est appellée à chaque frame dans un beginDrawing() de raylib
 * - cleanup est appellée lorsque le programme quitte cet état
 * 
 */
typedef struct GameState {
  void (*init)(GameContext *ctx);
  void (*update)(GameContext *ctx);
  void (*draw)(GameContext *ctx);
  void (*cleanup)(GameContext *ctx);
} GameState;

/**
 * @brief Un GameContext contient l'ensemble des variables globales du jeu à un moment donné.
 * IL est subdivisé en différents systèmes, dont en particulier la pool, ayant chacun
 * leurs propres données.
 * Il contient également les identifiants de l'état actuel du programme et de l'état suivant.
 * L'état suivant ne prend en général sa valeur qu'au moment du changement d'état.
 * 
 */
struct GameContext {
  StateID currentStateID;
  StateID nextStateID;
  GameState *currentState;

  /* Systemes */
  InputSystem input;
  ButtonSystem button;
  ScoreSystem score;
  ScreenSystem screen;

  Pool * pool;

  CoSched sched;

  int debug;
  int pause;

  /* Audio */
  float volume_bgm;
  float volume_sfx;
};

/**
 * @brief Initialise le système d'états et bascule vers l'état demandé.
 *
 * Initialise les structures internes du GameContext 
 * puis effectue la transition vers state (appel de la
 * fonction init associée à l'état).
 *
 * @param ctx ctx à initialiser
 * @param state id de l'état initial à activer
 */
extern void gamestate_initialize(GameContext *ctx, StateID state);

/**
 * @brief Appelée chaque frame pour mettre à jour l'état courant.
 *
 * Appelle la fonction update du GameState actif et gère la
 * transition d'état si nextStateID diffère de currentStateID.
 *
 * @param ctx GameContext courant
 */
extern void gamestate_update(GameContext *ctx);

/**
 * @brief Détecte si le menu pause doit être activé et si oui gêre sa logique.
 * Cette fonction doit être appellée à chaque frame.
 *
 * @param ctx GameContext courant
 */
void pauseListener(GameContext *ctx);

/**
 * @brief Affiche et le menu de pause.
 *
 * Doit être appelé uniquement lorsque le jeu est en état de pause.
 *
 * @param ctx GameContext courant
 */
void pauseMenu(GameContext *ctx);

/**
 * @brief Demande une transition vers un autre état de jeu.
 *
 *
 * @param ctx GameContext courant
 * @param stateID Identifiant de l'état cible.
 */
extern void gamestate_change_state(GameContext *ctx, StateID stateID);

extern GameState state_menu_title;
extern GameState state_menu_keybinds;
extern GameState state_test;
extern GameState state_moonlight;
extern GameState state_menu_settings;
extern GameState state_game_over;
extern GameState state_victory;
