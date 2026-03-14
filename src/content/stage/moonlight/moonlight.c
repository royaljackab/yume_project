#include "content/stage/moonlight/moonlight.h"
#include "core/assets.h"
#include "game_state.h"
#include "physics.h"
#include "player.h"
#include "pool.h"
#include "sprite.h"
#include "components/bullet.h"
#include "components/looseLaser.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

void state_moonlight_init(GameContext *ctx) {
    ctx->pool = malloc(sizeof(Pool));
    if (!ctx->pool) {
        printf("FATAL ERROR: Moonlight pool allocation failed\n");
        return;
    }

    pool_init(ctx->pool);
    Player_start(ctx->pool, TEST_PLAYER, DEFAULT_PATTERN);

    float angleT = 90;
    for (int i=0; i < 10; i++) {
        Bullet_enemy_spawn(ctx->pool, 100, 100, 3, angleT, BULLET_FIRE_BLUE);
        
        angleT += 36;
    }
    loose_laser_create(ctx->pool, 100,100, 3, 50, 10, 1000, BLUE); //Amori

    StopMusicStream(playlist[BGM_WAITING]);
    PlayMusicStream(playlist[BGM_FAST_DANGER]);
}

void state_moonlight_update(GameContext *ctx) {
    Player_update(ctx);
    Physics_update_all(ctx->pool);
    loose_lasers_update_all(ctx->pool); //Amori
    pool_kill_convicts(ctx->pool);
}

void state_moonlight_draw(GameContext *ctx) {
    ClearBackground(BLACK);

    Sprite_draw_all(ctx->pool);
    draw_all_loose_lasers(&ctx->pool->looseLaser,&ctx->pool->position); //Amori


    DrawText("coucou", 30, 30, 50, WHITE);
}

void state_moonlight_cleanup(GameContext *ctx) {
    StopMusicStream(playlist[BGM_FAST_DANGER]);
    free(ctx->pool);
}

GameState state_moonlight = {state_moonlight_init, state_moonlight_update, state_moonlight_draw, state_moonlight_cleanup};
