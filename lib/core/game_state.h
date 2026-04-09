#pragma once

#include "core/input.h"
#include "coroutine/cosched.h"
#include "ecs/pool.h"
#include "systems/button.h"
#include "systems/score.h"
#include <raylib.h>
#include "../systems/screen.h"

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

typedef struct GameState {
  void (*init)(GameContext *ctx);
  void (*update)(GameContext *ctx);
  void (*draw)(GameContext *ctx);
  void (*cleanup)(GameContext *ctx);
} GameState;

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

extern void gamestate_initialize(GameContext *ctx, StateID state);
extern void gamestate_update(GameContext *ctx);
void pauseListener(GameContext *ctx);
void pauseMenu(GameContext *ctx);

extern void gamestate_change_state(GameContext *ctx, StateID stateID);

extern GameState state_menu_title;
extern GameState state_menu_keybinds;
extern GameState state_test;
extern GameState state_moonlight;
extern GameState state_menu_settings;
extern GameState state_game_over;
extern GameState state_victory;
