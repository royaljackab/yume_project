/**
 * @file state_menu_settings.c
 * @brief Écran de réglages audio, navigation via ButtonSystem.
 */

#include "content/ui/state_menu_settings.h"
#include "core/game_state.h"
#include "core/settings.h"
#include "content/assets.h"
#include "systems/button.h"
#include "components/background.h"
#include "core/coroutine/tasks.h"
#include <raylib.h>
#include <raymath.h>

#include <stdio.h>

/* --- Enum des boutons --- */
typedef enum {
  BTN_VOLUME_BGM = 0,
  BTN_VOLUME_SFX,
  BTN_ENREGISTRER,
  BTN_KEYBINDS,
  BTN_RETOUR,
  NB_BTN_SETTINGS
} SettingsBtn;

#define PAS_VOLUME 0.05f
#define BARRE_LARGEUR 400
#define BARRE_HAUTEUR 25

/* ------------------------------------------------------------------ */
/* Fonctions internes                                                  */
/* ------------------------------------------------------------------ */

void audio_appliquer_volumes(GameContext *ctx) {
  /* Applique les volumes stockés dans le contexte à tous les sons chargés */
  for (int i = 0; i < MAX_BGM; i++)
    SetMusicVolume(playlist[i], ctx->volume_bgm);
  for (int i = 0; i < MAX_SFX; i++)
    SetSoundVolume(sfx[i], ctx->volume_sfx);
}

static void dessiner_barre_volume(float valeur, int x, int y, Color couleur) {
  /* Fond gris */
  DrawRectangle(x, y, BARRE_LARGEUR, BARRE_HAUTEUR, DARKGRAY);
  /* Remplissage proportionnel */
  DrawRectangle(x, y, (int)(BARRE_LARGEUR * valeur), BARRE_HAUTEUR, couleur);
  /* Bordure */
  DrawRectangleLines(x, y, BARRE_LARGEUR, BARRE_HAUTEUR, WHITE);
  /* Pourcentage */
  DrawText(TextFormat("%d%%", (int)(valeur * 100)), x + BARRE_LARGEUR + 15, y + 3, 20, WHITE);
}

/* ------------------------------------------------------------------ */
/* Fonctions state                                                     */
/* ------------------------------------------------------------------ */

void state_menu_settings_init(GameContext *ctx) {
  ctx->pool = malloc(sizeof(Pool));
  if (!ctx->pool) {
      printf("FATAL ERROR: menu_settings pool allocation failed\n");
      return;
  }

  pool_init(ctx->pool);
  button_system_init(&ctx->button);

  /* Create background */
  Entity bg = invoke_main_background(ctx->pool);
  (void)bg; /* Background entity for future use */

  /* Positions Y de chaque option */
  button_create(&ctx->button, 50, 200);  /* BTN_VOLUME_BGM */
  button_create(&ctx->button, 50, 340);  /* BTN_VOLUME_SFX */
  button_create(&ctx->button, 50, 470);  /* BTN_ENREGISTRER */
  button_create(&ctx->button, 50, 570);  /* BTN_KEYBINDS   */
  button_create(&ctx->button, 50, 620);  /* BTN_RETOUR      */

  FontsLoad();
  audio_appliquer_volumes(ctx);
}

void state_menu_settings_update(GameContext *ctx) {
  /* Navigation haut/bas + validation gérées par button_update */
  button_update(&ctx->button, &ctx->input);

  int sel = button_get_current_buttonID(&ctx->button);

  /* Gauche/droite : modifier le volume du bouton sélectionné */
  if (sel == BTN_VOLUME_BGM) {
    if (isPressed(ctx->input.right)) {
      ctx->volume_bgm = Clamp(ctx->volume_bgm + PAS_VOLUME, 0.0f, 1.0f);
      audio_appliquer_volumes(ctx);
    }
    if (isPressed(ctx->input.left)) {
      ctx->volume_bgm = Clamp(ctx->volume_bgm - PAS_VOLUME, 0.0f, 1.0f);
      audio_appliquer_volumes(ctx);
    }
  }

  if (sel == BTN_VOLUME_SFX) {
    if (isPressed(ctx->input.right)) {
      ctx->volume_sfx = Clamp(ctx->volume_sfx + PAS_VOLUME, 0.0f, 1.0f);
      audio_appliquer_volumes(ctx);
      PlaySound(sfx[SFX_MENU_NAV]); /* test sonore immédiat */
    }
    if (isPressed(ctx->input.left)) {
      ctx->volume_sfx = Clamp(ctx->volume_sfx - PAS_VOLUME, 0.0f, 1.0f);
      audio_appliquer_volumes(ctx);
      PlaySound(sfx[SFX_MENU_NAV]);
    }
  }

  /* Validation (Espace) */
  if (button_is_validated(&ctx->button)) {
    switch (sel) {
    case BTN_KEYBINDS:
      gamestate_change_state(ctx, STATE_MENU_KEYBINDS);
      break;
    case BTN_ENREGISTRER:
      saveSettings(ctx);
      break;
    case BTN_RETOUR:
      gamestate_change_state(ctx, STATE_MENU_TITLE);
      break;
    default:
      break; /* volumes : espace ne fait rien de spécial */
    }
  }

  Background_update_all(ctx->pool);

  /* en dernier par sécurité */
  pool_kill_convicts(ctx->pool);
}

void state_menu_settings_draw(GameContext *ctx) {
  ClearBackground(BLACK);
  Sprite_draw_range(ctx->pool, -50, -1);

  
  Vector2 vect = {50, 60};
  DrawTextEx(fonts[TOUHOU_98],"REGLAGES", vect , 50, 3 ,RED);

  int sel = button_get_current_buttonID(&ctx->button);

  /* --- Volume BGM --- */
  button_draw_button_text(&ctx->button, BTN_VOLUME_BGM,
    "Volume musique", 40,
    (sel == BTN_VOLUME_BGM) ? YELLOW : GRAY);
  dessiner_barre_volume(ctx->volume_bgm, 50, 250, RED);
  if (sel == BTN_VOLUME_BGM)
    DrawText("< / >", 50, 282, 16, YELLOW);

  /* --- Volume SFX --- */
  button_draw_button_text(&ctx->button, BTN_VOLUME_SFX,
    "Volume effets sonores", 40,
    (sel == BTN_VOLUME_SFX) ? YELLOW : GRAY);
  dessiner_barre_volume(ctx->volume_sfx, 50, 390, (Color){80, 0, 120, 255});
  if (sel == BTN_VOLUME_SFX)
    DrawText("< / >", 50, 422, 16, YELLOW);

  /* --- Controles clavier --- */
  button_draw_button_text_touhou98(&ctx->button, BTN_KEYBINDS,
    "Controles clavier", 30,
    (sel == BTN_KEYBINDS) ? YELLOW : GRAY);

  /* --- Enregistrer --- */
  button_draw_button_text(&ctx->button, BTN_ENREGISTRER,
    "Enregistrer", 40,
    (sel == BTN_ENREGISTRER) ? YELLOW : GRAY);

  /* --- Retour --- */
  button_draw_button_text_touhou98(&ctx->button, BTN_RETOUR,
    "Retour", 30,
    (sel == BTN_RETOUR) ? YELLOW : GRAY);

  /* Curseur * */
  button_draw_selector_text(&ctx->button, -35, 0, "*", 40, YELLOW);

  /* Aide en bas */
  DrawText("Haut/Bas : naviguer   Gauche/Droite : modifier volume   Espace : valider",
           50, 680, 18, DARKGRAY);
}

void state_menu_settings_cleanup(GameContext *ctx) {
  cosched_finish(&ctx->sched);
  free(ctx->pool);
}

GameState state_menu_settings = {
  .init    = state_menu_settings_init,
  .update  = state_menu_settings_update,
  .draw    = state_menu_settings_draw,
  .cleanup = state_menu_settings_cleanup
};