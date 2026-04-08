/**
 * @file state_game_over.c
 * @brief Écran de fin de partie (Game Over)
 * @author @amyelalem
 */

#include <stdio.h>
#include "content/ui/state_game_over.h"
#include "core/game_state.h"
#include "screen.h"
#include "core/input.h"
#include <raylib.h>

static int timer = 0;

void state_game_over_init(GameContext *ctx) {
    timer = 0;
    (void)ctx;
}

void state_game_over_update(GameContext *ctx) {
    timer++;
    /* Appuyer sur Espace après 1 seconde pour retourner au menu */
    if (timer > 60 && IsKeyPressed(ctx->input.keybinds.validate)) {
        gamestate_change_state(ctx, STATE_MENU_TITLE);
    }
}

void state_game_over_draw(GameContext *ctx) {
    ClearBackground(BLACK);

    int cx = PANEL_WIDTH / 2;

    DrawText("GAME OVER",
             cx - MeasureText("GAME OVER", 80) / 2,
             200, 80, RED);

    char score_text[64];
    sprintf(score_text, "Score final : %09d", ctx->score.score);
    DrawText(score_text,
             cx - MeasureText(score_text, 30) / 2,
             320, 30, WHITE);

    if (timer > 60) {
        DrawText("Appuyer sur Espace pour revenir au menu",
                 cx - MeasureText("Appuyer sur Espace pour revenir au menu", 20) / 2,
                 420, 20, GRAY);
    }
}

void state_game_over_cleanup(GameContext *ctx) {
    (void)ctx;
}

GameState state_game_over = {
    state_game_over_init,
    state_game_over_update,
    state_game_over_draw,
    state_game_over_cleanup
};