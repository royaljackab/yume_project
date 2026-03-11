#include "content/stage/moonlight/moonlight.h"
#include "assets.h"
#include "game_state.h"
#include "physics.h"
#include "player.h"
#include "pool.h"
#include "sprite.h"
#include "components/bullet.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "components/enemy.h"

void state_moonlight_init(GameContext *ctx) {
    ctx->pool = malloc(sizeof(Pool));
    if (!ctx->pool) {
        printf("FATAL ERROR: Moonlight pool allocation failed\n");
        return;
    }

    pool_init(ctx->pool);
    Player_start(ctx->pool, TEST_PLAYER, DEFAULT_PATTERN);

    Enemy_spawn(ctx->pool, 200, -30, 2.0f, ENEMY_FAIRY_BLUE, PATTERN_STRAIGHT_DOWN, 3);
    Enemy_spawn(ctx->pool, 400, -60, 2.0f, ENEMY_FAIRY_BLUE, PATTERN_ZIGZAG, 3);
    Enemy_spawn(ctx->pool, 600, -90, 2.0f, ENEMY_FAIRY_BLUE, PATTERN_CIRCLE_IN, 3);

    float angleT = 90;
    for (int i=0; i < 10; i++) {
        Bullet_enemy_spawn(ctx->pool, 100, 100, 3, angleT, ANIM_TEST);
        angleT += 36;
    }
}

void state_moonlight_update(GameContext *ctx) {
    Player_update(ctx);
    Physics_update_all(ctx->pool);
    Enemy_update_all(ctx->pool);
    pool_kill_convicts(ctx->pool);
}

void state_moonlight_draw(GameContext *ctx) {
    ClearBackground(BLACK);

    Sprite_draw_all(ctx->pool);
    DrawText("coucou", 30, 30, 50, WHITE);
}

void state_moonlight_cleanup(GameContext *ctx) {
    free(ctx->pool);
}

GameState state_moonlight = {state_moonlight_init, state_moonlight_update, state_moonlight_draw, state_moonlight_cleanup};