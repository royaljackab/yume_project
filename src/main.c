#include "content/assets.h"
#include "core/game_state.h"
#include "core/screen.h"
#include "core/settings.h"
#include "cotask_data.h"
#include <stddef.h>
#include <stdio.h>

int main() {
  InitAudioDevice();
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Night of nights");
  SetTargetFPS(60);

  AssetsLoad();
  SpritesLoad();

  cotask_global_init();

  GameContext ctx;
  gamestate_initialize(&ctx, STATE_MENU_TITLE);
  ctx.pause = 0;

  input_initialize(&ctx.input);
  load_settings(&ctx);

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
      pauseMenu();
    }

    if (ctx.currentState != NULL) {
      ctx.currentState->update(&ctx);

      BeginDrawing();
        ctx.currentState->draw(&ctx);
      EndDrawing();
    }
  }

  // AssetsUnload();
  CloseAudioDevice();

  cotask_global_shutdown();
  CloseWindow();
}
