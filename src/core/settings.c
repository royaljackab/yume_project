#include "core/settings.h"
#include "core/game_state.h"
#include <stdio.h>
#include <string.h>

void init_settings(GameContext *ctx) {
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

  // ------ audio ------
  ctx->volume_bgm = 0.3f;
  ctx->volume_sfx = 0.8f;
  fprintf(new, "volume_bgm=%d\n", (int)(ctx->volume_bgm * 100));
  fprintf(new, "volume_sfx=%d\n", (int)(ctx->volume_sfx * 100));

  // ------ écran ------
  fprintf(new, "screen_width=%d\n", ctx->screen.screen_width);
  fprintf(new, "screen_height=%d\n", ctx->screen.screen_height);

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
      ctx->input.keybinds.left = optionValue;
    } else if (strcmp(optionName, "keybind_move_right") == 0) {
      ctx->input.keybinds.right = optionValue;
    } else if (strcmp(optionName, "keybind_move_down") == 0) {
      ctx->input.keybinds.down = optionValue;
    } else if (strcmp(optionName, "keybind_move_up") == 0) {
      ctx->input.keybinds.up = optionValue;
    } else if (strcmp(optionName, "keybind_shoot") == 0) {
      ctx->input.keybinds.shoot = optionValue;
    } else if (strcmp(optionName, "keybind_pause") == 0) {
      ctx->input.keybinds.pause = optionValue;
    } else if (strcmp(optionName, "keybind_focus_mode") == 0) {
      ctx->input.keybinds.focus = optionValue;
    } else if (strcmp(optionName, "keybind_validate") == 0) {
      ctx->input.keybinds.validate = optionValue;
    } else if (strcmp(optionName, "keybind_bomb") == 0) {
      ctx->input.keybinds.bomb = optionValue;
    } else if (strcmp(optionName, "keybind_skip") == 0) {
      ctx->input.keybinds.skip = optionValue;
    } else if (strcmp(optionName, "volume_bgm") == 0) {
      ctx->volume_bgm = optionValue / 100.0f;
    } else if (strcmp(optionName, "volume_sfx") == 0) {
      ctx->volume_sfx = optionValue / 100.0f;
    } else if (strcmp(optionName, "screen_width") == 0) {
      ctx->screen.screen_width = optionValue;
    } else if (strcmp(optionName, "screen_height") == 0) {
      ctx->screen.screen_height = optionValue;
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
    } else if (strcmp(optionName, "volume_bgm") == 0) {
      fprintf(new, "%s=%d\n", optionName, (int)(ctx->volume_bgm * 100));
    } else if (strcmp(optionName, "volume_sfx") == 0) {
      fprintf(new, "%s=%d\n", optionName, (int)(ctx->volume_sfx * 100));
    } else if (strcmp(optionName, "screen_width") == 0) {
      fprintf(new, "%s=%d\n", optionName, ctx->screen.screen_width);
    } else if (strcmp(optionName, "screen_height") == 0) {
      fprintf(new, "%s=%d\n", optionName, ctx->screen.screen_height);
    }
  }
  fclose(old);
  fclose(new);

  remove("../settings.txt");
  rename("../temp.txt", "../settings.txt");
}

