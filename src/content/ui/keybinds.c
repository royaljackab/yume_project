#include "content/ui/keybinds.h"
#include "core/game_state.h"
#include "core/input.h"
#include "core/screen.h"
#include "core/settings.h"
#include "systems/button.h"
#include "core/coroutine/tasks.h"

#include <raylib.h>
#include <stdio.h>

// fonctions state

typedef enum {
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
  RETOUR,
  NB_KEYBINDS_BUTTONS
} KeybindsBtn;

#define ACTION_BUTTON_COUNT 10  // Nombre de boutons d'actions (jusqu'à SKIP)

static int capture_active = 0;
static int capture_action = -1;
static KeyboardKey capture_validate_key = KEY_NULL;

/* ------------------------------------------------------------------ */
/* Fonctions internes                                                  */
/* ------------------------------------------------------------------ */

static const char *get_action_name(int buttonID) {
  switch (buttonID) {
    case MOVE_LEFT: return "Deplacement gauche";
    case MOVE_RIGHT: return "Deplacement droite";
    case MOVE_DOWN: return "Deplacement bas";
    case MOVE_UP: return "Deplacement haut";
    case SHOOT: return "Tirer";
    case FOCUS_MODE: return "Mode focus";
    case VALIDATE: return "Valider";
    case PAUSE: return "Pause";
    case BOMB: return "Bombe";
    case SKIP: return "Passer";
    default: return "";
  }
}

static KeyboardKey *get_keybind_ptr(GameContext *ctx, int buttonID) {
  switch (buttonID) {
    case MOVE_LEFT: return &ctx->input.keybinds.left;
    case MOVE_RIGHT: return &ctx->input.keybinds.right;
    case MOVE_DOWN: return &ctx->input.keybinds.down;
    case MOVE_UP: return &ctx->input.keybinds.up;
    case SHOOT: return &ctx->input.keybinds.shoot;
    case FOCUS_MODE: return &ctx->input.keybinds.focus;
    case VALIDATE: return &ctx->input.keybinds.validate;
    case PAUSE: return &ctx->input.keybinds.pause;
    case BOMB: return &ctx->input.keybinds.bomb;
    case SKIP: return &ctx->input.keybinds.skip;
    default: return NULL;
  }
}

static const char *get_key_name(KeyboardKey key) {
  switch (key) {
    case KEY_SPACE: return "SPACE";
    case KEY_ESCAPE: return "ESC";
    case KEY_ENTER: return "ENTER";
    case KEY_TAB: return "TAB";
    case KEY_BACKSPACE: return "BACKSPACE";
    case KEY_INSERT: return "INS";
    case KEY_DELETE: return "DEL";
    case KEY_RIGHT: return "RIGHT";
    case KEY_LEFT: return "LEFT";
    case KEY_DOWN: return "DOWN";
    case KEY_UP: return "UP";
    case KEY_PAGE_UP: return "PAGE UP";
    case KEY_PAGE_DOWN: return "PAGE DOWN";
    case KEY_HOME: return "HOME";
    case KEY_END: return "END";
    case KEY_CAPS_LOCK: return "CAPS LOCK";
    case KEY_SCROLL_LOCK: return "SCROLL LOCK";
    case KEY_NUM_LOCK: return "NUM LOCK";
    case KEY_PRINT_SCREEN: return "PRINT SCREEN";
    case KEY_PAUSE: return "PAUSE";
    case KEY_F1: return "F1";
    case KEY_F2: return "F2";
    case KEY_F3: return "F3";
    case KEY_F4: return "F4";
    case KEY_F5: return "F5";
    case KEY_F6: return "F6";
    case KEY_F7: return "F7";
    case KEY_F8: return "F8";
    case KEY_F9: return "F9";
    case KEY_F10: return "F10";
    case KEY_F11: return "F11";
    case KEY_F12: return "F12";
    case KEY_LEFT_SHIFT: return "LEFT SHIFT";
    case KEY_LEFT_CONTROL: return "LEFT CTRL";
    case KEY_LEFT_ALT: return "LEFT ALT";
    case KEY_LEFT_SUPER: return "LEFT SUPER";
    case KEY_RIGHT_SHIFT: return "RIGHT SHIFT";
    case KEY_RIGHT_CONTROL: return "RIGHT CTRL";
    case KEY_RIGHT_ALT: return "RIGHT ALT";
    case KEY_RIGHT_SUPER: return "RIGHT SUPER";
    case KEY_KB_MENU: return "KB MENU";
    case KEY_KP_0: return "KP 0";
    case KEY_KP_1: return "KP 1";
    case KEY_KP_2: return "KP 2";
    case KEY_KP_3: return "KP 3";
    case KEY_KP_4: return "KP 4";
    case KEY_KP_5: return "KP 5";
    case KEY_KP_6: return "KP 6";
    case KEY_KP_7: return "KP 7";
    case KEY_KP_8: return "KP 8";
    case KEY_KP_9: return "KP 9";
    case KEY_KP_DECIMAL: return "KP .";
    case KEY_KP_DIVIDE: return "KP /";
    case KEY_KP_MULTIPLY: return "KP *";
    case KEY_KP_SUBTRACT: return "KP -";
    case KEY_KP_ADD: return "KP +";
    case KEY_KP_ENTER: return "KP ENTER";
    case KEY_KP_EQUAL: return "KP =";
    case KEY_BACK: return "BACK";
    case KEY_MENU: return "MENU";
    case KEY_VOLUME_UP: return "VOLUME UP";
    case KEY_VOLUME_DOWN: return "VOLUME DOWN";
    case KEY_A: return "A";
    case KEY_B: return "B";
    case KEY_C: return "C";
    case KEY_D: return "D";
    case KEY_E: return "E";
    case KEY_F: return "F";
    case KEY_G: return "G";
    case KEY_H: return "H";
    case KEY_I: return "I";
    case KEY_J: return "J";
    case KEY_K: return "K";
    case KEY_L: return "L";
    case KEY_M: return "M";
    case KEY_N: return "N";
    case KEY_O: return "O";
    case KEY_P: return "P";
    case KEY_Q: return "Q";
    case KEY_R: return "R";
    case KEY_S: return "S";
    case KEY_T: return "T";
    case KEY_U: return "U";
    case KEY_V: return "V";
    case KEY_W: return "W";
    case KEY_X: return "X";
    case KEY_Y: return "Y";
    case KEY_Z: return "Z";
    case KEY_ZERO: return "0";
    case KEY_ONE: return "1";
    case KEY_TWO: return "2";
    case KEY_THREE: return "3";
    case KEY_FOUR: return "4";
    case KEY_FIVE: return "5";
    case KEY_SIX: return "6";
    case KEY_SEVEN: return "7";
    case KEY_EIGHT: return "8";
    case KEY_NINE: return "9";
    case KEY_APOSTROPHE: return "'";
    case KEY_COMMA: return ",";
    case KEY_MINUS: return "-";
    case KEY_PERIOD: return ".";
    case KEY_SLASH: return "/";
    case KEY_SEMICOLON: return ";";
    case KEY_EQUAL: return "=";
    case KEY_LEFT_BRACKET: return "[";
    case KEY_BACKSLASH: return "\\";
    case KEY_RIGHT_BRACKET: return "]";
    case KEY_GRAVE: return "`";
    default: return "?";
  }
}

TASK(keybind_capture, { GameContext *ctx; int action; KeyboardKey validate_key; }) {
  capture_active = 1;
  capture_action = ARGS.action;
  capture_validate_key = ARGS.validate_key;

  while (IsKeyDown(ARGS.validate_key)) {
    YIELD;
  }

  int assigned = 0;
  while (!WindowShouldClose() && !assigned) {
    if (IsKeyPressed(KEY_ESCAPE)) {
      break;
    }

    for (int k = KEY_SPACE; k <= KEY_KB_MENU; k++) {
      if ((KeyboardKey)k == ARGS.validate_key) continue;
      if (IsKeyPressed((KeyboardKey)k)) {
        KeyboardKey *key_ptr = get_keybind_ptr(ARGS.ctx, ARGS.action);
        if (key_ptr) {
          input_change_keybind(key_ptr, (KeyboardKey)k);
        }
        assigned = 1;
        break;
      }
    }

    if (!assigned) {
      YIELD;
    }
  }

  capture_active = 0;
  capture_action = -1;
  capture_validate_key = KEY_NULL;
}

/* ------------------------------------------------------------------ */
/* Fonctions state                                                     */
/* ------------------------------------------------------------------ */




void state_menu_keybinds_init(GameContext *ctx) {
  button_system_init(&ctx->button);
  cosched_init(&ctx->sched, NULL);
  capture_active = 0;
  capture_action = -1;
  capture_validate_key = KEY_NULL;

  /* Positions Y pour les actions */
  int y = 150;
  for (int i = 0; i < ACTION_BUTTON_COUNT; i++) {
    button_create(&ctx->button, 50, y);
    y += 40;
  }

  /* Bouton Retour */
  button_create(&ctx->button, 50, 550);
}

void state_menu_keybinds_update(GameContext *ctx) {
  if (capture_active) {
    cosched_run_tasks(&ctx->sched);
    return;
  }

  button_update(&ctx->button, &ctx->input);

  if (button_is_validated(&ctx->button)) {
    int sel = button_get_current_buttonID(&ctx->button);
    if (sel < ACTION_BUTTON_COUNT) {
      capture_active = 1;
      capture_action = sel;
      capture_validate_key = ctx->input.keybinds.validate;
      SCHED_INVOKE_TASK(&ctx->sched, keybind_capture, ctx, sel, capture_validate_key);
      cosched_run_tasks(&ctx->sched);
      return;
    } else if (sel == RETOUR) {
      gamestate_change_state(ctx, STATE_MENU_SETTINGS);
    }
  }
}

void state_menu_keybinds_draw(GameContext *ctx) {
  ClearBackground(BLACK);

  DrawText("CONTROLES CLAVIER", 50, 60, 70, RED);

  int sel = button_get_current_buttonID(&ctx->button);

  /* Dessiner les actions */
  for (int i = 0; i < ACTION_BUTTON_COUNT; i++) {
    Color color = (sel == i) ? YELLOW : GRAY;
    button_draw_button_text(&ctx->button, i, (char *)get_action_name(i), 30, color);

    // Dessiner la touche actuelle à côté
    KeyboardKey *key_ptr = get_keybind_ptr(ctx, i);
    if (key_ptr) {
      const char *key_name = get_key_name(*key_ptr);
      DrawText(key_name, 450, ctx->button.buttons[i].pos.y + 5, 30, color);
    }
  }

  /* Bouton Retour */
  button_draw_button_text(&ctx->button, RETOUR,
    "Retour", 40,
    (sel == RETOUR) ? YELLOW : GRAY);

  /* Curseur * */
  button_draw_selector_text(&ctx->button, -35, 0, "*", 40, YELLOW);

  /* Aide en bas */
  DrawText("Haut/Bas : naviguer   Espace : valider/changer touche",
           50, 680, 18, DARKGRAY);

  if (capture_active) {
    Rectangle rec = {120, 300, 560, 160};
    DrawRectangleRec(rec, (Color){20, 20, 20, 220});
    DrawRectangleLinesEx(rec, 3, WHITE);
    DrawText("Relâchez la touche de validation actuelle...", 140, 320, 24, WHITE);
    DrawText(TextFormat("Nouvelle touche pour '%s' :", get_action_name(capture_action)), 140, 360, 24, WHITE);
    DrawText("Appuyez sur une touche", 140, 400, 20, GRAY);
  }
}

void state_menu_keybinds_cleanup(GameContext *ctx) {
  cosched_finish(&ctx->sched);
}

GameState state_menu_keybinds = {
  .init    = state_menu_keybinds_init,
  .update  = state_menu_keybinds_update,
  .draw    = state_menu_keybinds_draw,
  .cleanup = state_menu_keybinds_cleanup
};
