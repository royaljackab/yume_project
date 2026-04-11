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
/**
 * @brief Initialise l'état de victoire
 * @param ctx Le contexte du jeu
 */
    StopMusicStream(playlist[BGM_MORIYA_THEME]);
    PlayMusicStream(playlist[BGM_WAITING]);

    ctx->pool = malloc(sizeof(Pool));
    if (!ctx->pool) {
        printf("FATAL ERROR: victory pool allocation failed\n");
        return;
    }

    pool_init(ctx->pool);
    cosched_init(&ctx->sched, ctx->pool);

    Entity bg = invoke_main_background(ctx->pool, &ctx->screen);
    (void)bg;

    timer = 0;
    FontsLoad();
}

void state_victory_update(GameContext *ctx) {
/**
 * @brief Met à jour l'état de victoire
 * @param ctx Le contexte du jeu
 */
    timer++;
    if (timer > 60 && IsKeyPressed(ctx->input.keybinds.validate)) {
        gamestate_change_state(ctx, STATE_MENU_TITLE);
    }

    Background_update_all(ctx->pool);

    /* en dernier par sécurité */
    pool_kill_convicts(ctx->pool);
}

void state_victory_draw(GameContext *ctx) {
/**
 * @brief Dessine l'écran de victoire, est appelé a chaque frame
 * @param ctx Le contexte du jeu
 */
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
/**
 * @brief Nettoie l'état de fin de partie en cas de victoire (enregistre le highscore)
 * @param ctx Le contexte du jeu
 */
    update_highscore(ctx->score.score); // Met à jour le highscore si nécessaire
    (void)ctx;
    cosched_finish(&ctx->sched);
    free(ctx->pool);
}

GameState state_victory = {
    state_victory_init,
    state_victory_update,
    state_victory_draw,
    state_victory_cleanup
};