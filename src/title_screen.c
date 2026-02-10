#include "../lib/title_screen.h"
#include "../lib/button.h"
#include "../lib/game_state.h"
#include <raylib.h>
#include <stdio.h>

/* Enum */
typedef enum TitleButtons {
    BEGIN_GAME,
    MOONLIGHT_TEST,
    KEYBINDS
} TitleButtons;

/* Fonctions static */

/* Fonctions state */
void state_menu_title_init(GameContext *ctx){

  /* Systeme des boutons */
  button_system_init(&ctx->button);
  button_create(&ctx->button, 50, 400);
  button_create(&ctx->button, 50, 500);
  button_create(&ctx->button, 50, 600);
}

void state_menu_title_draw(GameContext *ctx){
  ClearBackground(BLACK);

  DrawText("TOUHOU_C", 50, 200, 80, RED);
    
  // Affichage des boutons
  button_draw_button_text(&ctx->button, BEGIN_GAME, "BEGIN GAME", 50, WHITE);
  button_draw_button_text(&ctx->button, MOONLIGHT_TEST, "MOONLIGHT_TEST", 50, WHITE);
  button_draw_button_text(&ctx->button, KEYBINDS, "SETTINGS", 50, WHITE);

  if (button_is_button_validated(&ctx->button, BEGIN_GAME)) {
    Vector2 pos = button_get_button_position(&ctx->button, BEGIN_GAME);
    DrawText("NOT IMPLEMENTED", pos.x + 500, pos.y, 50, RED);
  }

  button_draw_selector_text(&ctx->button, -30, 0, "*", 50, WHITE);
}

void state_menu_title_update(GameContext *ctx){
  /* Systeme boutons */
  button_update(&ctx->button,&ctx->input);

  if(button_is_validated(&ctx->button)) {
    switch(button_get_current_buttonID(&ctx->button)) {
      case BEGIN_GAME:
        break;
      case MOONLIGHT_TEST:
        gamestate_change_state(ctx, STATE_TEST);
        break;
      case KEYBINDS:
        gamestate_change_state(ctx, STATE_MENU_KEYBINDS);
        break;
    }
  }
}

GameState state_menu_title = {
  .init = state_menu_title_init,
  .update = state_menu_title_update,
  .draw = state_menu_title_draw,
  .cleanup = NULL
};


