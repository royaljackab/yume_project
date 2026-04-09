#include "content/ui/title_screen.h"
#include "core/game_state.h"
#include "systems/button.h"
#include "components/background.h"
#include "core/coroutine/tasks.h"
#include "components/background.h"
#include "core/coroutine/tasks.h"
#include <raylib.h>
#include <stdio.h>
#include "content/assets.h"

/* Enum */
typedef enum TitleButtons { BEGIN_GAME, SETTINGS } TitleButtons;

/* Fonctions static */

/* Fonctions state */
void state_menu_title_init(GameContext *ctx) {
  ctx->pool = malloc(sizeof(Pool));
  if (!ctx->pool) {
      printf("FATAL ERROR: title_screen pool allocation failed\n");
      return;
  }

  pool_init(ctx->pool);
  cosched_init(&ctx->sched, ctx->pool);

  /* Create background */
  Entity bg = invoke_main_background(ctx->pool, &ctx->screen);

  /* Systeme des boutons */
  button_system_init(&ctx->button);
  button_create(&ctx->button, 50, 400);
  button_create(&ctx->button, 50, 500);

  FontsLoad();
  PlayMusicStream(playlist[BGM_WAITING]);
  SetMusicVolume(playlist[BGM_WAITING], ctx->volume_bgm);
}

void state_menu_title_draw(GameContext *ctx) {
  ClearBackground(BLACK);
  Sprite_draw_range(ctx->pool, -50, -1);

  Vector2 vect = {50, 200};
  DrawTextEx(fonts[TOUHOU_98],"YUME PROJECT", vect , 70, 3 ,RED);

  // Affichage des boutons
  button_draw_button_text_touhou98(&ctx->button, BEGIN_GAME, "Play", 30, WHITE);
  button_draw_button_text_touhou98(&ctx->button, SETTINGS, "Settings", 30, WHITE);
  button_draw_selector_text(&ctx->button, -30, 0, "*", 50, WHITE);
}

void state_menu_title_update(GameContext *ctx) {
  /* Systeme boutons */
  button_update(&ctx->button, &ctx->input);

  if (button_is_validated(&ctx->button)) {
    switch (button_get_current_buttonID(&ctx->button)) {
    case BEGIN_GAME:
      gamestate_change_state(ctx, STATE_MOONLIGHT);
      break;
    case SETTINGS:
      gamestate_change_state(ctx, STATE_MENU_SETTINGS);
      break;
    }
  }

  Background_update_all(ctx->pool);

  /* en dernier par sécurité */
  pool_kill_convicts(ctx->pool);
}

void state_menu_title_cleanup(GameContext *ctx) {
  cosched_finish(&ctx->sched);
  free(ctx->pool);
}

GameState state_menu_title = {.init = state_menu_title_init,
                              .update = state_menu_title_update,
                              .draw = state_menu_title_draw,
                              .cleanup = state_menu_title_cleanup};

