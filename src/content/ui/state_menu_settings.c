/**
 * @file state_menu_settings.c
 * @brief Écran de réglages audio : volume BGM et SFX, navigable au clavier.
 */

#include "content/ui/state_menu_settings.h"
#include "core/game_state.h"
#include "core/settings.h"
#include "content/assets.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

/* Options du menu */
typedef enum {
  OPT_VOLUME_BGM = 0,
  OPT_VOLUME_SFX,
  OPT_KEYBINDS,
  OPT_RETOUR,
  NB_OPTIONS
} SettingsOption;

static int option_courante = 0;

/* ------------------------------------------------------------------ */
/* Fonctions internes                                                  */
/* ------------------------------------------------------------------ */

void audio_appliquer_volumes(GameContext *ctx) {
  /* Applique les volumes stockés dans le contexte à tous les sons chargés */
  for (int i = 0; i < MAX_BGM; i++) {
    SetMusicVolume(playlist[i], ctx->volume_bgm);
  }
  for (int i = 0; i < MAX_SFX; i++) {
    SetSoundVolume(sfx[i], ctx->volume_sfx);
  }
}

static void dessiner_barre(float valeur, int x, int y, int largeur, int hauteur) {
  /* Fond gris */
  DrawRectangle(x, y, largeur, hauteur, DARKGRAY);
  /* Remplissage rose proportionnel à la valeur */
  DrawRectangle(x, y, (int)(largeur * valeur), hauteur, PINK);
  /* Bordure blanche */
  DrawRectangleLines(x, y, largeur, hauteur, WHITE);
  /* Pourcentage à droite */
  DrawText(TextFormat("%d%%", (int)(valeur * 100)), x + largeur + 15, y, hauteur, WHITE);
}

/* ------------------------------------------------------------------ */
/* Fonctions state                                                     */
/* ------------------------------------------------------------------ */

void state_menu_settings_init(GameContext *ctx) {
  option_courante = 0;
  /* Applique les volumes dès l'entrée dans le menu (chargés depuis fichier) */
  audio_appliquer_volumes(ctx);
}

void state_menu_settings_update(GameContext *ctx) {
  const float PAS = 0.05f; /* 5% par appui */

  /* Navigation haut/bas avec IsKeyPressed direct (menu, pas gameplay) */
  if (IsKeyPressed(ctx->input.keybinds.up)) {
    option_courante = (option_courante - 1 + NB_OPTIONS) % NB_OPTIONS;
    PlaySound(sfx[SFX_MENU_NAV]);
  }
  if (IsKeyPressed(ctx->input.keybinds.down)) {
    option_courante = (option_courante + 1) % NB_OPTIONS;
    PlaySound(sfx[SFX_MENU_NAV]);
  }

  /* --- Modification volume BGM --- */
  if (option_courante == OPT_VOLUME_BGM) {
    if (isPressed(ctx->input.right)) {
      ctx->volume_bgm = Clamp(ctx->volume_bgm + PAS, 0.0f, 1.0f);
      audio_appliquer_volumes(ctx);
    }
    if (isPressed(ctx->input.left)) {
      ctx->volume_bgm = Clamp(ctx->volume_bgm - PAS, 0.0f, 1.0f);
      audio_appliquer_volumes(ctx);
    }
  }

  /* --- Modification volume SFX --- */
  if (option_courante == OPT_VOLUME_SFX) {
    if (isPressed(ctx->input.right)) {
      ctx->volume_sfx = Clamp(ctx->volume_sfx + PAS, 0.0f, 1.0f);
      audio_appliquer_volumes(ctx);
      /* Jouer un son de test pour entendre le nouveau niveau */
      PlaySound(sfx[SFX_MENU_NAV]);
    }
    if (isPressed(ctx->input.left)) {
      ctx->volume_sfx = Clamp(ctx->volume_sfx - PAS, 0.0f, 1.0f);
      audio_appliquer_volumes(ctx);
      PlaySound(sfx[SFX_MENU_NAV]);
    }
  }

  /* --- Validation --- */
  if (isPressed(ctx->input.validate)) {
    switch (option_courante) {
    case OPT_KEYBINDS:
      gamestate_change_state(ctx, STATE_MENU_KEYBINDS);
      break;
    case OPT_RETOUR:
      gamestate_change_state(ctx, STATE_MENU_TITLE);
      break;
    default:
      break;
    }
  }

  /* Raccourci retour rapide avec skip (Ctrl) */
  if (isPressed(ctx->input.skip)) {
    gamestate_change_state(ctx, STATE_MENU_TITLE);
  }
}

void state_menu_settings_draw(GameContext *ctx) {
  ClearBackground(BLACK);

  DrawText("REGLAGES", 50, 60, 70, RED);

  /* --- Volume BGM --- */
  Color c_bgm = (option_courante == OPT_VOLUME_BGM) ? YELLOW : GRAY;
  DrawText("*", 20, 200, 40, (option_courante == OPT_VOLUME_BGM) ? YELLOW : BLACK);
  DrawText("Volume musique", 50, 200, 40, c_bgm);
  DrawRectangle(50, 250, 400, 25, DARKGRAY);
  DrawRectangle(50, 250, (int)(400 * ctx->volume_bgm), 25, RED);
  DrawRectangleLines(50, 250, 400, 25, WHITE);
  DrawText(TextFormat("%d%%", (int)(ctx->volume_bgm * 100)), 465, 252, 20, WHITE);
  /* Hint SOUS la barre, pas à côté */
  if (option_courante == OPT_VOLUME_BGM)
    DrawText("< gauche / droite >", 50, 282, 16, YELLOW);

  /* --- Volume SFX --- */
  Color c_sfx = (option_courante == OPT_VOLUME_SFX) ? YELLOW : GRAY;
  DrawText("*", 20, 330, 40, (option_courante == OPT_VOLUME_SFX) ? YELLOW : BLACK);
  DrawText("Volume effets sonores", 50, 330, 40, c_sfx);
  DrawRectangle(50, 380, 400, 25, DARKGRAY);
  DrawRectangle(50, 380, (int)(400 * ctx->volume_sfx), 25, (Color){80, 0, 120, 255});
  DrawRectangleLines(50, 380, 400, 25, WHITE);
  DrawText(TextFormat("%d%%", (int)(ctx->volume_sfx * 100)), 465, 382, 20, WHITE);
  if (option_courante == OPT_VOLUME_SFX)
    DrawText("< gauche / droite >", 50, 412, 16, YELLOW);

  /* --- Controles clavier --- */
  Color c_kb = (option_courante == OPT_KEYBINDS) ? YELLOW : GRAY;
  DrawText("*", 20, 450, 40, (option_courante == OPT_KEYBINDS) ? YELLOW : BLACK);
  DrawText("Controles clavier", 50, 450, 40, c_kb);

  /* --- Retour --- */
  Color c_ret = (option_courante == OPT_RETOUR) ? YELLOW : GRAY;
  DrawText("*", 20, 520, 40, (option_courante == OPT_RETOUR) ? YELLOW : BLACK);
  DrawText("Retour", 50, 520, 40, c_ret);

  DrawText("Haut/Bas : naviguer   Gauche/Droite : modifier   Espace : valider",
           50, 680, 18, DARKGRAY);
}

void state_menu_settings_cleanup(GameContext *ctx) {
  /* Sauvegarde automatique en quittant le menu */
  saveSettings(ctx);
}

GameState state_menu_settings = {
  .init    = state_menu_settings_init,
  .update  = state_menu_settings_update,
  .draw    = state_menu_settings_draw,
  .cleanup = state_menu_settings_cleanup
};
