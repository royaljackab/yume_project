#include "../lib/game_state.h"
#include "../lib/settings.h"
#include <stddef.h>
#include <stdio.h>

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Night of nights");
  SetTargetFPS(60);

  GameContext ctx;
  gamestate_initialize(&ctx, STATE_MENU_TITLE);
  ctx.pause = 0;
  
  input_initialize(&ctx.input);
  load_settings(&ctx);

  // Load_BulletSprites();
  // Load_PlayerSprites();
  // Load_Players();
  // InitPlayer(REIMU_A);
  // AssetsLoad();
  // initialize();

  
  

  void *moonlightState = NULL;

  ctx.currentState->init(&ctx);

  while (!WindowShouldClose()) {
    input_update(&ctx.input);
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

  AssetsUnload();
  CloseWindow();
}

//   while (!WindowShouldClose()) {
//     input_update(&globals.inputState);
//     switch (globals.gameState) {
//     case TITLE_SCREEN:
//       menu();
//       break;
//     case MOONLIGHT:

//       if (!globals.pause) {
//         UpdateObjects();
//         moonlight_task(&moonlightState);
//         UpdatePlayer();
//         UpdateAnimations();
//         UpdateAnimationPlayer();
//         UpdateCollisions();
//       }

//       BeginDrawing();
//       ClearBackground(BLACK);
//       DrawRectangleLines(PANEL_LEFT, PANEL_UP, PANEL_WIDTH, PANEL_HEIGHT,
//                          WHITE);

//       DrawObjects();
//       DrawPlayer();
//       EndDrawing();
//       break;
//     default:
//       break;
//     }
//     if (globals.gameState != TITLE_SCREEN) {
//       pauseGame();
//     }
//     if (globals.pause) {
//       pauseMenu();
//     }
//   }
//   AssetsUnload();
//   CloseWindow();
// }
