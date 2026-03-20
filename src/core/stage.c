#include "stage.h"
#include "components/looseLaser.h"
#include "components/straight_laser.h"
#include "components/bullet.h"
#include "components/player.h"
#include "core/game_state.h"


void state_generic_init(GameContext *ctx, PlayerName player, PatternType type) {
    ctx->pool = malloc(sizeof(Pool));
    if (!ctx->pool) {
        printf("FATAL ERROR: generic pool allocation failed\n");
        return;
    }
    
    pool_init(ctx->pool);
    Player_start(ctx->pool, player, type);
}

void stage_generic_update(GameContext *ctx) {

    Player_update(ctx);
    Physics_update_all(ctx->pool);
    loose_lasers_update_all(ctx->pool); 
    straight_lasers_update_all(ctx->pool);
    Owner_update(ctx->pool); 
    pool_kill_convicts(ctx->pool);
}

void stage_generic_draw(GameContext *ctx) {
    ClearBackground(BLACK);
    Sprite_draw_all(ctx->pool);
    draw_all_loose_lasers(&ctx->pool->looseLaser,&ctx->pool->position); 
    straight_lasers_draw_all(&ctx->pool->straightLaser,&ctx->pool->position,&ctx->pool->sprite); 
}

void stage_generic_cleanup(GameContext *ctx) {
    StopMusicStream(playlist[BGM_FAST_DANGER]);
    free(ctx->pool);
}