#include "content/assets.h"
#include "content/ui/state_menu_settings.h"
#include "core/game_state.h"
#include "screen.h"
#include "core/settings.h"
#include "cotask_data.h"
#include <stddef.h>
#include <stdio.h>

int main() {

  GameContext ctx;
  screen_system_init(&ctx.screen); 

  InitAudioDevice();
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Night of nights");
  SetTargetFPS(60);

  AssetsLoad();
  SpritesLoad();

  cotask_global_init();
  
  gamestate_initialize(&ctx, STATE_MENU_TITLE);
  ctx.pause = 0;

  input_initialize(&ctx.input);
  load_settings(&ctx);
  SetWindowSize(ctx.screen.screen_width,ctx.screen.screen_height);
  audio_appliquer_volumes(&ctx);

  ctx.currentState->init(&ctx);

  while (!WindowShouldClose()) {
    // WARNING: D'ou vient ca???? Je ne trouve pas le fichier a include
    // UpdateMusicStream(playlist[BGM_FAST_DANGER]);

    input_update(&ctx.input);
    
    // Mise à jour du stream audio
    for (int i = 0; i < MAX_BGM; i++) {
        UpdateMusicStream(playlist[i]);
    }
    gamestate_update(&ctx);

    if (ctx.currentStateID > __END_MENU__) {
      pauseListener(&ctx);
    }

    if (ctx.pause) {
      /* Jeu figé : on ne update plus, on dessine juste le menu pause */
      BeginDrawing();
        ctx.currentState->draw(&ctx);   // le stage reste visible en fond
        pauseMenu(&ctx);                // par-dessus
      EndDrawing();
    } else {
      /* Jeu actif : comportement normal */
      if (ctx.currentState != NULL) {
        ctx.currentState->update(&ctx);
        BeginDrawing();
          ctx.currentState->draw(&ctx);
        EndDrawing();
      }
    }
  }

  // AssetsUnload();
  CloseAudioDevice();

  cotask_global_shutdown();
  CloseWindow();
}
