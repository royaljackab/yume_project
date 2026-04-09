/**
 * @file state_victory.c
 * @brief Écran de victoire
 * @author @amyelalem
 */

#include "content/ui/state_victory.h"
#include "systems/screen.h"
#include "../lib/systems/screen.h"
#include "core/input.h"
#include "content/assets.h"
#include "components/background.h"
#include "core/coroutine/tasks.h"
#include <raylib.h>
#include <stdio.h>
#include "core/highscore.h"

    
static int timer = 0;

void state_victory_init(GameContext *ctx) {
    ctx->pool = malloc(sizeof(Pool));
    if (!ctx->pool) {
        printf("FATAL ERROR: victory pool allocation failed\n");
        return;
    }

    pool_init(ctx->pool);

    /* Create background */
    Entity bg = invoke_main_background(ctx->pool, &ctx->screen);
    (void)bg; /* Background entity for future use */

    timer = 0;
    FontsLoad();
}

void state_victory_update(GameContext *ctx) {
    timer++;
    if (timer > 60 && IsKeyPressed(ctx->input.keybinds.validate)) {
        gamestate_change_state(ctx, STATE_MENU_TITLE);
    }

    Background_update_all(ctx->pool);

    /* en dernier par sécurité */
    pool_kill_convicts(ctx->pool);
}

void state_victory_draw(GameContext *ctx) {
    ClearBackground(BLACK);
    Sprite_draw_range(ctx->pool, -50, -1);

    int cx = PANEL_WIDTH / 2;

    DrawText("CONGRATULATIONS !",
             cx - MeasureText("CONGRATULATIONS !", 60) / 2,
             180, 60, GOLD);

    DrawText("Vous avez vaincu le boss !",
             cx - MeasureText("Vous avez vaincu le boss !", 30) / 2,
             280, 30, WHITE);

    char score_text[64];
    sprintf(score_text, "Score final : %09d", ctx->score.score);
    DrawText(score_text,
             cx - MeasureText(score_text, 30) / 2,
             340, 30, WHITE);

    if (update_highscore(ctx->score.score)) { //affiche le message de nouveau record
        DrawText("Nouveau highscore !",
                 cx - MeasureText("Nouveau highscore !", 30) / 2,
                 360, 30, YELLOW);
    }

    if (timer > 60) {
        DrawText("Appuyer sur Espace pour revenir au menu",
                 cx - MeasureText("Appuyer sur Espace pour revenir au menu", 20) / 2,
                 440, 20, GRAY);
    }
}

void state_victory_cleanup(GameContext *ctx) {
    cosched_finish(&ctx->sched);
    free(ctx->pool);
}

GameState state_victory = {
    state_victory_init,
    state_victory_update,
    state_victory_draw,
    state_victory_cleanup
};