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
#include "core/highscore.h"
#include "content/assets.h"
#include "components/background.h"
#include "core/coroutine/tasks.h"
#include <raylib.h>
#include <stdio.h>

static int timer = 0;

void state_game_over_init(GameContext *ctx) {
/**
 * @param ctx Le contexte du jeu
 * @brief Initialise l'état de fin de partie
 */
    StopMusicStream(playlist[BGM_MORIYA_THEME]);
    PlayMusicStream(playlist[BGM_WAITING]);

    ctx->pool = malloc(sizeof(Pool));
    if (!ctx->pool) {
        printf("FATAL ERROR: game over pool allocation failed\n");
        return;
    }

    pool_init(ctx->pool);
    cosched_init(&ctx->sched, ctx->pool);

    /* Create background */
    Entity bg = invoke_main_background(ctx->pool, &ctx->screen);
    (void)bg; /* Background entity for future use */

    timer = 0;
    FontsLoad();
}
 
void state_game_over_update(GameContext *ctx) {
    timer++;
    /* Appuyer sur Espace après 1 seconde pour retourner au menu */
    if (timer > 60 && IsKeyPressed(ctx->input.keybinds.validate)) {
        gamestate_change_state(ctx, STATE_MENU_TITLE);
    }

    Background_update_all(ctx->pool);

    /* en dernier par sécurité */
    pool_kill_convicts(ctx->pool);
}

void state_game_over_draw(GameContext *ctx) {
/**
 * @brief Dessine l'écran de fin de partie (défaite)
 * @param ctx Le contexte du jeu
 */

    ClearBackground(BLACK);
    Sprite_draw_range(ctx->pool, -50, -1);

    int cx = PANEL_WIDTH / 2;

    DrawText("GAME OVER",
             cx - MeasureText("GAME OVER", 80) / 2,
             200, 80, RED);

    char score_text[64];
    sprintf(score_text, "Score final : %09d", ctx->score.score);
    DrawText(score_text,
             cx - MeasureText(score_text, 30) / 2,
             320, 30, WHITE);

    if (score_system_is_new_highscore(&ctx->score)) { //affiche le message de nouveau record
        DrawText("Nouveau highscore !",
                 cx - MeasureText("Nouveau highscore !", 30) / 2,
                 360, 30, YELLOW);
    }


    if (timer > 60) {
        DrawText("Appuyer sur Espace pour revenir au menu",
                 cx - MeasureText("Appuyer sur Espace pour revenir au menu", 20) / 2,
                 420, 20, GRAY);
    }
}

void state_game_over_cleanup(GameContext *ctx) {
<<<<<<< HEAD
/**
 * @brief Nettoie l'état de fin de partie en cas de défaite (enregistre le highscore)
 * @param ctx Le contexte du jeu
 */
    update_highscore(ctx->score.score); // Met à jour le highscore si nécessaire
    (void)ctx;
=======
    cosched_finish(&ctx->sched);
    free(ctx->pool);
>>>>>>> origin/main
}

GameState state_game_over = {
    state_game_over_init,
    state_game_over_update,
    state_game_over_draw,
    state_game_over_cleanup
};