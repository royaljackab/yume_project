#include "core/game_state.h"
#include "core/input.h"
#include "core/screen.h"

#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include <stdio.h>
#include "content/ui/state_menu_settings.h"
#include "content/ui/state_game_over.h"

// Variable statique pour savoir ce qu'on modifie en pause
// 0 = BGM, 1 = SFX
static int pause_selection = 0;

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
  case STATE_MENU_SETTINGS:
    return &state_menu_settings;
  case STATE_MOONLIGHT:
    return &state_moonlight;
  case STATE_GAME_OVER:
    return &state_game_over;
  
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

void gamestate_change_state(GameContext *ctx, StateID stateID) {
  /***
   * A utiliser pour changer d'état
   */

  ctx->nextStateID = stateID;
}

void pauseListener(GameContext *ctx) {
  const float PAS = 0.05f;

  if (IsKeyPressed(ctx->input.keybinds.pause)) {
    ctx->pause = !ctx->pause;
    pause_selection = 0; // reset à BGM à chaque ouverture
  }

  if (ctx->pause) {
    /* Haut/Bas : changer la sélection */
    if (IsKeyPressed(ctx->input.keybinds.up)) {
      pause_selection = (pause_selection - 1 + 2) % 2;
    }
    if (IsKeyPressed(ctx->input.keybinds.down)) {
      pause_selection = (pause_selection + 1) % 2;
    }

    /* Gauche/Droite : modifier le volume sélectionné */
    if (IsKeyPressed(ctx->input.keybinds.right)) {
      if (pause_selection == 0) ctx->volume_bgm = Clamp(ctx->volume_bgm + PAS, 0.0f, 1.0f);
      else                      ctx->volume_sfx = Clamp(ctx->volume_sfx + PAS, 0.0f, 1.0f);
      audio_appliquer_volumes(ctx);
    }
    if (IsKeyPressed(ctx->input.keybinds.left)) {
      if (pause_selection == 0) ctx->volume_bgm = Clamp(ctx->volume_bgm - PAS, 0.0f, 1.0f);
      else                      ctx->volume_sfx = Clamp(ctx->volume_sfx - PAS, 0.0f, 1.0f);
      audio_appliquer_volumes(ctx);
    }
  }
}

void pauseMenu(GameContext *ctx) {
  /* Fond semi-transparent */
  DrawRectangle(50, 50, PANEL_WIDTH - 100, PANEL_HEIGHT - 100, (Color){0, 0, 0, 180});
  DrawRectangleLines(50, 50, PANEL_WIDTH - 100, PANEL_HEIGHT - 100, WHITE);

  DrawText("PAUSE", 100, 120, 60, RED);

  /* --- Volume BGM --- */
  Color c_bgm = (pause_selection == 0) ? YELLOW : WHITE;
  DrawText("Volume musique", 100, 230, 30, c_bgm);
  DrawText("*", 70, 230, 30, (pause_selection == 0) ? YELLOW : BLACK);
  DrawRectangle(100, 270, 400, 20, DARKGRAY);
  DrawRectangle(100, 270, (int)(400 * ctx->volume_bgm), 20, RED);
  DrawRectangleLines(100, 270, 400, 20, WHITE);
  DrawText(TextFormat("%d%%", (int)(ctx->volume_bgm * 100)), 515, 270, 20, WHITE);

  /* --- Volume SFX --- */
  Color c_sfx = (pause_selection == 1) ? YELLOW : WHITE;
  DrawText("Volume effets sonores", 100, 320, 30, c_sfx);
  DrawText("*", 70, 320, 30, (pause_selection == 1) ? YELLOW : BLACK);
  DrawRectangle(100, 360, 400, 20, DARKGRAY);
  DrawRectangle(100, 360, (int)(400 * ctx->volume_sfx), 20, (Color){80, 0, 120, 255});
  DrawRectangleLines(100, 360, 400, 20, WHITE);
  DrawText(TextFormat("%d%%", (int)(ctx->volume_sfx * 100)), 515, 360, 20, WHITE);

  DrawText("Haut/Bas : changer   Gauche/Droite : modifier   P : reprendre",
           100, PANEL_HEIGHT - 120, 18, DARKGRAY);
}
