#include "../../lib/game_state.h"
#include "../../lib/input.h"

#include <stddef.h>
#include <stdio.h>


GameState *get_state_pointer(StateID state) {
  /***
   * @return: pointeur vers l'état désigné par state
   */

  // NOTE: Nouvel état = Nouveau case ici !
  switch (state) {
  case STATE_MENU_TITLE:
    return &state_menu_title;
  case STATE_MENU_KEYBINDS:
    return &state_menu_keybinds;
  case STATE_TEST:
    // return &state_test;
  default:
    return 0;
  }
}

void gamestate_initialize(GameContext *ctx, StateID init_state) {
  /*** Initialise le Game System à un state
   *
   */

  ctx->nextStateID = STATE_NONE;
  ctx->currentStateID = init_state;
  ctx->currentState = get_state_pointer(init_state);
}

void gamestate_update(GameContext *ctx) {
  /***
   * Appelée a chaque frame pour effectuer le changement d'état (s'il a lieu)
   */

  // Pas de changement d'état
  if (ctx->nextStateID == STATE_NONE) {
    return;
  }

  // On nettoie l'état précédent
  if (ctx->currentState->cleanup != NULL) {
    ctx->currentState->cleanup(ctx);
  }
  // Changer l'ID et pointeur
  ctx->currentStateID = ctx->nextStateID;
  ctx->currentState = get_state_pointer(ctx->currentStateID);
  printf("Switched to state %d\n", ctx->currentStateID);

  // Initialiser le nouvel etat
  if (ctx->currentState != NULL) {
    ctx->currentState->init(ctx);
  }

  // Reset trigger
  ctx->nextStateID = STATE_NONE;
}

void gamestate_change_state(GameContext * ctx, StateID stateID) {
  /*** 
  * A utiliser pour changer d'état
  */

  ctx->nextStateID = stateID;
}

void pauseListener(GameContext *ctx){
    if(isPressed(ctx->input.pause)){
        ctx->pause = !ctx->pause;
    }
}

void pauseMenu(){
    DrawText("PAUSE", 100, 300, 50, RED);
    DrawRectangleLines(50, 50, PANEL_WIDTH - 100, PANEL_HEIGHT - 100, WHITE);
}
