#include "core/settings.h"
#include "core/game_state.h"
#include <stdio.h>
#include <string.h>

// KEY_RIGHT           = 262,      // Key: Cursor right
// KEY_LEFT            = 263,      // Key: Cursor left
// KEY_DOWN            = 264,      // Key: Cursor down
// KEY_UP              = 265,      // Key: Cursor up

void init_settings(GameContext *ctx) {
  // créé le fichier settings.txt si il n'existe pas.

  //------------ Valeurs par défaut des options ------------
  //------ keybinds ------
  ctx->input.keybinds.left = KEY_LEFT;
  ctx->input.keybinds.right = KEY_RIGHT;
  ctx->input.keybinds.down = KEY_DOWN;
  ctx->input.keybinds.up = KEY_UP;
  ctx->input.keybinds.shoot = KEY_Z;
  ctx->input.keybinds.pause = KEY_P;
  ctx->input.keybinds.focus = KEY_LEFT_SHIFT;
  ctx->input.keybinds.validate = KEY_SPACE;
  ctx->input.keybinds.bomb = KEY_X;
  ctx->input.keybinds.skip = KEY_LEFT_CONTROL;

  FILE *new = fopen("../settings.txt", "w");
  if (!new) {
    return;
  }
  //------------ Création du fichier ------------
  //------ keybinds ------
  fprintf(new, "keybind_move_left=%d\n", ctx->input.keybinds.left);
  fprintf(new, "keybind_move_right=%d\n", ctx->input.keybinds.right);
  fprintf(new, "keybind_move_down=%d\n", ctx->input.keybinds.down);
  fprintf(new, "keybind_move_up=%d\n", ctx->input.keybinds.up);
  fprintf(new, "keybind_shoot=%d\n", ctx->input.keybinds.shoot);
  fprintf(new, "keybind_pause=%d\n", ctx->input.keybinds.pause);
  fprintf(new, "keybind_focus_mode=%d\n", ctx->input.keybinds.focus);
  fprintf(new, "keybind_validate=%d\n", ctx->input.keybinds.validate);
  fprintf(new, "keybind_skip=%d\n", ctx->input.keybinds.skip);
  fprintf(new, "keybind_bomb=%d\n", ctx->input.keybinds.bomb);

  fclose(new);
}

void load_settings(GameContext *ctx) {
  char optionName[50];
  int optionValue;
  FILE *f = fopen("../settings.txt", "r");

  if (!f) {
    init_settings(ctx);
    return;
  }
  printf("options chargées avec succès\n");
  while (fscanf(f, " %29[^=]=%d", optionName, &optionValue) != EOF) {
    //------ keybinds ------
    if (strcmp(optionName, "keybind_move_left") == 0) {
      printf("left set to %d\n", optionValue);
      ctx->input.keybinds.left = optionValue;
    } else if (strcmp(optionName, "keybind_move_right") == 0) {
      printf("right set to %d\n", optionValue);
      ctx->input.keybinds.right = optionValue;
    } else if (strcmp(optionName, "keybind_move_down") == 0) {
      printf("down set to %d\n", optionValue);
      ctx->input.keybinds.down = optionValue;
    } else if (strcmp(optionName, "keybind_move_up") == 0) {
      printf("up set to %d\n", optionValue);
      ctx->input.keybinds.up = optionValue;
    } else if (strcmp(optionName, "keybind_shoot") == 0) {
      printf("shoot set to %d\n", optionValue);
      ctx->input.keybinds.shoot = optionValue;
    } else if (strcmp(optionName, "keybind_pause") == 0) {
      printf("pause set to %d\n", optionValue);
      ctx->input.keybinds.pause = optionValue;
    } else if (strcmp(optionName, "keybind_focus_mode") == 0) {
      printf("focus mode set to %d\n", optionValue);
      ctx->input.keybinds.focus = optionValue;
    } else if (strcmp(optionName, "keybind_validate") == 0) {
      printf("validate set to %d\n", optionValue);
      ctx->input.keybinds.validate = optionValue;
    } else if (strcmp(optionName, "keybind_bomb") == 0) {
      printf("bomb set to %d\n", optionValue);
      ctx->input.keybinds.bomb = optionValue;
    } else if (strcmp(optionName, "keybind_skip") == 0) {
      printf("skip set to %d\n", optionValue);
      ctx->input.keybinds.skip = optionValue;
    }
  }
  fclose(f);
}

void saveSettings(GameContext *ctx) {
  FILE *new = fopen("../temp.txt", "w");
  char optionName[50];
  FILE *old = fopen("../settings.txt", "r");
  int dummy;
  if (!old || !new) {
    return;
  }

  while (fscanf(old, " %29[^=]=%d", optionName, &dummy) != EOF) {
    //------ keybinds ------
    if (strcmp(optionName, "keybind_move_left") == 0) {
      fprintf(new, "%s=%d\n", optionName, ctx->input.keybinds.left);
    } else if (strcmp(optionName, "keybind_move_right") == 0) {
      fprintf(new, "%s=%d\n", optionName, ctx->input.keybinds.right);
    } else if (strcmp(optionName, "keybind_move_down") == 0) {
      fprintf(new, "%s=%d\n", optionName, ctx->input.keybinds.down);
    } else if (strcmp(optionName, "keybind_move_up") == 0) {
      fprintf(new, "%s=%d\n", optionName, ctx->input.keybinds.up);
    } else if (strcmp(optionName, "keybind_shoot") == 0) {
      fprintf(new, "%s=%d\n", optionName, ctx->input.keybinds.shoot);
    } else if (strcmp(optionName, "keybind_pause") == 0) {
      fprintf(new, "%s=%d\n", optionName, ctx->input.keybinds.pause);
    } else if (strcmp(optionName, "keybind_focus_mode") == 0) {
      fprintf(new, "%s=%d\n", optionName, ctx->input.keybinds.focus);
    } else if (strcmp(optionName, "keybind_validate") == 0) {
      fprintf(new, "%s=%d\n", optionName, ctx->input.keybinds.validate);
    } else if (strcmp(optionName, "keybind_bomb") == 0) {
      fprintf(new, "%s=%d\n", optionName, ctx->input.keybinds.bomb);
    } else if (strcmp(optionName, "keybind_skip") == 0) {
      fprintf(new, "%s=%d\n", optionName, ctx->input.keybinds.skip);
    }
  }
  fclose(old);
  fclose(new);

  remove("../settings.txt");
  rename("../temp.txt", "../settings.txt");
}
