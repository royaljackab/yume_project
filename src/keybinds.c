#include "../lib/keybinds.h"
#include "../lib/button.h"
#include "../lib/settings.h"
#include "../lib/game_state.h"
#include "../lib/input.h"
#include "../lib/globals.h"

#include <raylib.h>
#include <stdio.h>

//fonctions state 

enum {
    DONE,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_UP,
    SHOOT,
    FOCUS_MODE,
    VALIDATE,
    PAUSE,
    BOMB,
    SKIP,
    SAVE,
    NB_KEYBINDS_BUTTONS
};

#define DRAW_KEYBINDS(buttonID, text) button_draw_button_text(&ctx->button, buttonID, text, 20, WHITE)

/* Fonctions state */
void state_menu_keybinds_init(GameContext *ctx){
  button_system_init(&ctx->button);

  button_create(&ctx->button, 50, 400);
  
  int button_y = 450;
  for (int i=0; i<NB_KEYBINDS_BUTTONS; i++) {
    button_create(&ctx->button, 60, button_y);
    button_y += 20;
  }
  button_create(&ctx->button, 250, 400);
}

void state_menu_keybinds_draw(GameContext *ctx){
  ClearBackground(BLACK);

  DrawText("CONTROLS", 50, 200, 80, RED);
  DRAW_KEYBINDS(DONE, "done");
  DRAW_KEYBINDS(MOVE_LEFT, "left");
  DRAW_KEYBINDS(MOVE_RIGHT, "right");
  DRAW_KEYBINDS(MOVE_DOWN, "down");
  DRAW_KEYBINDS(MOVE_UP, "up");
  DRAW_KEYBINDS(SHOOT, "shoot");
  DRAW_KEYBINDS(FOCUS_MODE, "focus");
  DRAW_KEYBINDS(VALIDATE, "test");
  DRAW_KEYBINDS(PAUSE, "pause");
  DRAW_KEYBINDS(BOMB, "bomb");
  DRAW_KEYBINDS(SKIP, "skip");
  DRAW_KEYBINDS(SAVE, "save");
  button_draw_selector_text(&ctx->button, -30, 0, "*", 20, WHITE);

}

void state_menu_keybinds_update(GameContext *ctx){
    button_update(&ctx->button,&ctx->input);

  if(button_is_validated(&ctx->button)){
    switch(ctx->button.selection){
      case DONE:
        ctx->nextStateID = STATE_MENU_TITLE;
        break;
      case MOVE_LEFT:
        button_get_key("move_left", &ctx->input.keybinds.left);
        break;
      case MOVE_RIGHT:
        button_get_key("move_right", &ctx->input.keybinds.right);
        break;
      case MOVE_DOWN:
        button_get_key("move_down", &ctx->input.keybinds.down);
        break;
      case MOVE_UP:
        button_get_key("move_up", &ctx->input.keybinds.up);
        break;
      case SHOOT:
        button_get_key("shoot", &ctx->input.keybinds.shoot);
        break;
      case FOCUS_MODE:
        button_get_key("focus_mode", &ctx->input.keybinds.focus);
        break;
      case VALIDATE:
        button_get_key("validate", &ctx->input.keybinds.validate);
        break;
      case PAUSE:
        button_get_key("pause", &ctx->input.keybinds.pause);
        break;
      case BOMB:
        button_get_key("bomb", &ctx->input.keybinds.bomb);
        break;
      case SAVE:
        saveSettings(ctx);
        break;
    }
  }
}

GameState state_menu_keybinds = {
    .init = state_menu_keybinds_init,
    .update = state_menu_keybinds_update,
    .draw = state_menu_keybinds_draw,
    .cleanup = NULL
};

//Fonctions de boutons

// NOTE: Avoir un BeginDrawing hors d'une fonction draw n'est PAS tuff </3
void button_get_key(const char* target_name, KeyboardKey* key_target) {
    Rectangle rec = {100, 400, (float)PANEL_WIDTH - 200, 200};
    int key_assigned = 0;
    KeyboardKey pressedKey = KEY_NULL;
    
    while (GetKeyPressed() != 0) { }
    
    while (!WindowShouldClose() && !key_assigned) {
        pressedKey = GetKeyPressed();
        if (pressedKey != KEY_NULL && pressedKey != KEY_ESCAPE) {
            input_change_keybind(key_target, pressedKey);
            key_assigned = 1;
        }
        BeginDrawing();
            DrawRectangleRec(rec, BLACK);
            DrawRectangleLinesEx(rec, 3, WHITE);
            DrawText(TextFormat("Press the new key for '%s'", target_name), 120, 420, 20, WHITE);
            DrawText("(Press ESC to cancel)", 120, 460, 15, GRAY);
        EndDrawing();
    }
}
