#include "content/stage/moonlight/moonlight.h"
#include "core/assets.h"
#include "game_state.h"
#include "physics.h"
#include "player.h"
#include "pool.h"
#include "sprite.h"
#include "components/bullet.h"
#include "core/task.h"
#include "components/looseLaser.h"
#include "components/straight_laser.h"
#include "systems/hud.h"
#include "components/enemy.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

void * moonlight_ctx = NULL;

void state_moonlight_init(GameContext *ctx) {
    ctx->pool = malloc(sizeof(Pool));
    if (!ctx->pool) {
        printf("FATAL ERROR: Moonlight pool allocation failed\n");
        return;
    }
    
    pool_init(ctx->pool);
    Player_start(ctx->pool, TEST_PLAYER, DEFAULT_PATTERN);

    Enemy_spawn(ctx->pool, 480, 200, 0, 0, 5, 20,
        ENEMY_TYPE_FAIRY, ENEMY_FAIRY_BLUE_IDLE);
}

Define_Static_Task(fireRing, PARAMS(GameContext * gctx, int nb_ring, float angleT));
    for (int i=0; i < nb_ring; i++) {
        Bullet_enemy_spawn(gctx->pool, 1920 / 4, 100, 5, angleT, BALL_M_BLACK);
        angleT += 360.0 / nb_ring;
    }
End_Task;

Define_Static_Task(spiral, PARAMS(GameContext * gctx, float angvel, float angleT, int nb_ring, int periode), int count; float curr_angle; void * fireRing_ctx;);
    ctx->fireRing_ctx = NULL;
    ctx->count = 0;
    ctx->curr_angle = angleT;

    while(1) {
        if (ctx->count % periode == 0) {
            fireRing(&ctx->fireRing_ctx, gctx, nb_ring, ctx->curr_angle);
            ctx->curr_angle += angvel;
        }
        ctx->count++;
        yield;
    }
End_Task;

Define_Task(moonlight_task, PARAMS(GameContext *gctx), void * spiral_ctx; void *spiral_ctx_2;);
    ctx->spiral_ctx = NULL;
    ctx->spiral_ctx_2 = NULL;
    
    while(1) {
        spiral(&ctx->spiral_ctx, gctx, 3, 90, 30, 4);
        spiral(&ctx->spiral_ctx_2, gctx, -2, 90, 30, 4);
        yield;
    }
End_Task;

void state_moonlight_update(GameContext *ctx) {
    moonlight_task(&moonlight_ctx, ctx);

    Player_update(ctx);
    Physics_update_all(ctx->pool);
    loose_lasers_update_all(ctx->pool); //Amori
    straight_lasers_update_all(ctx->pool); //Amori
    pool_kill_convicts(ctx->pool);
    
    Enemy_update_all(ctx->pool);
}

void state_moonlight_draw(GameContext *ctx) {
    ClearBackground(BLACK);

    Sprite_draw_all(ctx->pool);
    HUD_draw(ctx, "Stage 1 - Moonlight");
    draw_all_loose_lasers(&ctx->pool->looseLaser,&ctx->pool->position); //Amori
    straight_lasers_draw_all(&ctx->pool->straightLaser,&ctx->pool->position,&ctx->pool->sprite); //Amori

    DrawText("coucou", 30, 30, 50, WHITE);
}

void state_moonlight_cleanup(GameContext *ctx) {
    StopMusicStream(playlist[BGM_FAST_DANGER]);
    free(ctx->pool);
}

GameState state_moonlight = {state_moonlight_init, state_moonlight_update, state_moonlight_draw, state_moonlight_cleanup};
