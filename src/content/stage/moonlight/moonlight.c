#include "content/stage/moonlight/moonlight.h"
#include "common.h"
#include "coroutine/cosched.h"
#include "ecs.h"
#include "content/assets.h"
#include "game_state.h"
#include "obj/obj.h"
#include "physics.h"
#include "player.h"
#include "pool.h"
#include "screen.h"
#include "sprite.h"
#include "components/bullet.h"
#include "core/coroutine/tasks.h"
#include "components/looseLaser.h"
#include "components/straight_laser.h"
#include "systems/hud.h"
#include "components/enemy.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static
int frames = 0;

TASK(crystal_wall, {GameContext *ctx;}) {
    const int num_crystals = 27;
    float spacing = PANEL_WIDTH / (float)(num_crystals - 1);
    float ofs = GetRandomValue(0, 5) - 1;

    for (int i=0; i < 30; i++) {
        for (int c = 0; c < num_crystals; ++c) {
            Vector2 accel = {0, 0.02 + 0.01 * ((c%2) ? 1 : -1) * sin((c*3+frames) / 30.0)};
            Entity bullet = Bullet_enemy_spawn(ARGS.ctx->pool, (ofs + c) * spacing + 10, PANEL_UP + 5, 0, 0, BULLET_BIG_RED);
            float r = (c % 2) ? 100 : 255;
            float g = (c % 2) ? 100 : 255;
            float b = (c % 2) ? 200 : 255;
            obj_SetColor(ARGS.ctx->pool, bullet, r, g, b);
            obj_SetForce(ARGS.ctx->pool, bullet, 0, accel.y);
        }

        WAIT(10);
    }
}

TASK(rotating_laser, { GameContext *ctx; }) {
    Entity laser = straight_laser_create(ARGS.ctx->pool, 500, 500, 90, 1500, 30, 5, 5, 10000, LASER_GOLD);
    TASK_BIND(laser);

    obj_SetAngularSpeed(ARGS.ctx->pool, laser, 0.2);
}

TASK(frostbolt, {GameContext *ctx; Vector2 pos; float angle; float speed;}) {
    Entity bullet = Bullet_enemy_spawn(ARGS.ctx->pool, ARGS.pos.x, ARGS.pos.y, ARGS.speed, ARGS.angle, ANIM_TEST);
    TASK_BIND(bullet);

    WAIT(300);
}

TASK(main_attack, {GameContext *ctx;}) {
    while (true)  {
        INVOKE_SUBTASK_DELAYED(60, crystal_wall, ARGS.ctx);
        WAIT(330);

        for (int t=0; t < 370; ++t) {
            if (!(t % 2)) {
                float speed = GetRandomValue(1, 400) / 100.0;
                float angle = atan2f( Player_GetY(ARGS.ctx->pool) - 500, Player_GetX(ARGS.ctx->pool) - 500) * RAD2DEG + GetRandomValue(-5, 5);
                INVOKE_TASK(frostbolt, ARGS.ctx,  {500, 500} , angle, speed);
            }

            WAIT(1);
        }
    }
}

void state_moonlight_init(GameContext *ctx) {
    ctx->pool = malloc(sizeof(Pool));
    if (!ctx->pool) {
        printf("FATAL ERROR: Moonlight pool allocation failed\n");
        return;
    }
    
    // Change the beat bruv
    StopMusicStream(playlist[BGM_WAITING]);
    PlayMusicStream(playlist[BGM_FAST_DANGER]);

    pool_init(ctx->pool);
    Player_start(ctx->pool, TEST_PLAYER, DEFAULT_PATTERN);

    cosched_init(&ctx->sched, ctx->pool);

    SCHED_INVOKE_TASK(&ctx->sched, main_attack, ctx);
}

void state_moonlight_update(GameContext *ctx) {
    cosched_run_tasks(&ctx->sched);

    Player_update(ctx);
    Physics_update_all(ctx->pool);
    loose_lasers_update_all(ctx->pool); 
    straight_lasers_update_all(ctx->pool);
    Owner_update(ctx->pool); 
    pool_kill_convicts(ctx->pool);

    Enemy_update_all(ctx->pool);

    frames++;
}

void state_moonlight_draw(GameContext *ctx) {
    ClearBackground(BLACK);

    Sprite_draw_all(ctx->pool);
    HUD_draw(ctx, "Stage 1 - Moonlight");
    draw_all_loose_lasers(&ctx->pool->looseLaser,&ctx->pool->position); 
    straight_lasers_draw_all(&ctx->pool->straightLaser,&ctx->pool->position,&ctx->pool->sprite); 
    

    DrawText("coucou", 30, 30, 50, WHITE);
}

void state_moonlight_cleanup(GameContext *ctx) {
    StopMusicStream(playlist[BGM_FAST_DANGER]);
    cosched_finish(&ctx->sched);
    free(ctx->pool);
}

GameState state_moonlight = {state_moonlight_init, state_moonlight_update, state_moonlight_draw, state_moonlight_cleanup};