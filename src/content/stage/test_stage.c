#include "content/stage/moonlight/moonlight.h"
#include "content/assets.h"
#include "game_state.h"
#include "physics.h"
#include "player.h"
#include "pool.h"
#include "sprite.h"
#include "components/bullet.h"
#include "core/task.h"

#include "components/looseLaser.h"
#include "components/straight_laser.h"
#include "stage.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

void * test_ctx = NULL;

//state functions

void test_stage_init(GameContext *ctx) {
    state_generic_init(ctx, TEST_PLAYER, DEFAULT_PATTERN);
}

void test_stage_update(GameContext *ctx) {
    stage_generic_update(ctx);
}

void test_stage_draw(GameContext *ctx) {
    stage_generic_draw(ctx);
    DrawText("coucou", 30, 30, 50, WHITE);
}

void test_stage_cleanup(GameContext *ctx) {
    stage_generic_cleanup(ctx);
}

GameState state_moonlight = {state_moonlight_init, state_moonlight_update, state_moonlight_draw, state_moonlight_cleanup};
