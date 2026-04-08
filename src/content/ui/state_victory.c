/**
 * @file state_victory.c
 * @brief Écran de victoire
 * @author @amyelalem
 */

#include "content/ui/state_victory.h"
#include "core/game_state.h"
#include "core/screen.h"
#include "core/input.h"
#include <raylib.h>
#include <stdio.h>

static int timer = 0;

void state_victory_init(GameContext *ctx) {
    timer = 0;
    (void)ctx;
}

void state_victory_update(GameContext *ctx) {
    timer++;
    if (timer > 60 && IsKeyPressed(ctx->input.keybinds.validate)) {
        gamestate_change_state(ctx, STATE_MENU_TITLE);
    }
}

void state_victory_draw(GameContext *ctx) {
    ClearBackground(BLACK);

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

    if (timer > 60) {
        DrawText("Appuyer sur Espace pour revenir au menu",
                 cx - MeasureText("Appuyer sur Espace pour revenir au menu", 20) / 2,
                 440, 20, GRAY);
    }
}

void state_victory_cleanup(GameContext *ctx) {
    (void)ctx;
}

GameState state_victory = {
    state_victory_init,
    state_victory_update,
    state_victory_draw,
    state_victory_cleanup
};