#include "content/assets.h"

#include "game_state.h"
#include "pool.h"
#include "core/task.h"

#include "components/physics.h"
#include "components/player.h"
#include "components/sprite.h"
#include "components/bullet.h"
#include "components/looseLaser.h"
#include "components/straight_laser.h"

#include "systems/stage.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

void * test_ctx = NULL;


Define_Static_Task(stage_wave_update, PARAMS(GameContext * gctx));
    int wave = gctx->stage.current_wave;
    
    switch(wave) {
        case DOWNTIME:
            break;
        case 1:
            break;
        case 2:
            break;
        case BOSS_WAVE:
            break;
        case STAGE_FINISHED:
            break;
    }

    if(wave != DOWNTIME && wave != BOSS_WAVE && wave != STAGE_FINISHED && wave_over(wave)) {
        gctx->stage.current_wave++;
    }
End_Task;


//state functions
void state_test_stage_init(GameContext *ctx) {
    state_generic_init(ctx, TEST_PLAYER, DEFAULT_PATTERN);
}

void state_test_stage_update(GameContext *ctx) {
    stage_generic_update(ctx);
}

void state_test_stage_draw(GameContext *ctx) {
    stage_generic_draw(ctx);
    DrawText("coucou", 30, 30, 50, WHITE);
}

void state_test_stage_cleanup(GameContext *ctx) {
    stage_generic_cleanup(ctx);
}

GameState state_test_stage = {state_test_stage_init, state_test_stage_update, state_test_stage_draw, state_test_stage_cleanup};
