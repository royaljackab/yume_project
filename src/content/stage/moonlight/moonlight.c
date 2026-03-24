#include "content/stage/moonlight/moonlight.h"
#include "common.h"
#include "core/assets.h"
#include "cosched.h"
#include "ecs.h"
#include "game_state.h"
#include "physics.h"
#include "player.h"
#include "pool.h"
#include "screen.h"
#include "sprite.h"
#include "components/bullet.h"
#include "core/coroutine/tasks.h"
#include "components/looseLaser.h"
#include "components/straight_laser.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

static
void fireRing(GameContext *ctx, float x, float y, int nb_ring, float angleT) {
    for (int i=0; i < nb_ring; i++) {
        Bullet_enemy_spawn(ctx->pool, x, y, 5, angleT, BALL_M_BLACK);
        angleT += 360.0 / nb_ring;
    }
}

TASK(rocket, {GameContext *ctx; float start_x; float start_y; float start_speed; }) {
    Pool *pool = ARGS.ctx->pool;
    Entity rocket = pool_create_entity(pool);
    TASK_BIND(rocket);

    Position pos = { .pos =  {ARGS.start_x, ARGS.start_y}, .angle = 270.0f };
    Position_add(&pool->position, rocket, pos);

    Physics phy = Physics_create_speed(ARGS.start_speed);

    Physics_set_accel(&phy, -0.4f);
    Physics_set_minSpd(&phy, 0);

    Physics_add(&pool->physics, rocket, phy);

    while (true) {
        Position *p = Position_get(&pool->position, rocket);
        Physics *ph = Physics_get(&pool->physics, rocket);

        Bullet_enemy_spawn(pool, p->pos.x, p->pos.y, 0, 0, ANIM_TEST);

        if (ph->speed <= 0) {
            break;
        }

        YIELD;
    }

    Position *final_pos = Position_get(&pool->position, rocket);
    float exp_x = final_pos->pos.x;
    float exp_y = final_pos->pos.y;

    fireRing(ARGS.ctx, exp_x, exp_y, 15, 7.0f);
    WAIT(8); 
    fireRing(ARGS.ctx, exp_x, exp_y, 15, 12.0f);

    pool_kill_entity(pool, rocket);
}

TASK(spiral, { GameContext *gctx; float x; float y; float angvel; float angleT; int nb_ring; int periode; int duration;}) {
    float curr_angle = ARGS.angleT;
    int frames_passed = 0;

    while(frames_passed < ARGS.duration) {
        fireRing(ARGS.gctx, ARGS.x, ARGS.y, ARGS.nb_ring, curr_angle);
        curr_angle += ARGS.angvel;

        WAIT(ARGS.periode);
        frames_passed += ARGS.periode;
    }
}

TASK(moonlight_task, {GameContext *gctx; }) {
    WAIT(60);

    while(true) {
        for (int i=0; i < 4; i++) {
            float random_x = GetRandomValue(PANEL_LEFT, PANEL_RIGHT);

            INVOKE_SUBTASK(rocket, ARGS.gctx, random_x, PANEL_DOWN, 5.0f);

            WAIT(30);
        }

        WAIT(120);

        INVOKE_SUBTASK(spiral, ARGS.gctx, 1920/2, 400, 3.5f, 0.0f, 8, 4, 240);
        INVOKE_SUBTASK(spiral, ARGS.gctx, 1920/2, 400, -3.5f, 180.0f, 8, 4, 240);

        WAIT(300);
    }
}

void state_moonlight_init(GameContext *ctx) {
    ctx->pool = malloc(sizeof(Pool));
    if (!ctx->pool) {
        printf("FATAL ERROR: Moonlight pool allocation failed\n");
        return;
    }
    
    pool_init(ctx->pool);
    Player_start(ctx->pool, TEST_PLAYER, DEFAULT_PATTERN);

    cosched_init(&ctx->sched, ctx->pool);

    SCHED_INVOKE_TASK(&ctx->sched, moonlight_task, ctx);
}



void state_moonlight_update(GameContext *ctx) {
    cosched_run_tasks(&ctx->sched);

    Player_update(ctx);
    Physics_update_all(ctx->pool);
    loose_lasers_update_all(ctx->pool); //Amori
    straight_lasers_update_all(ctx->pool); //Amori
    pool_kill_convicts(ctx->pool);
}

void state_moonlight_draw(GameContext *ctx) {
    ClearBackground(BLACK);

    Sprite_draw_all(ctx->pool);
    draw_all_loose_lasers(&ctx->pool->looseLaser,&ctx->pool->position); //Amori
    straight_lasers_draw_all(&ctx->pool->straightLaser,&ctx->pool->position,&ctx->pool->sprite); //Amori

    DrawText("coucou", 30, 30, 50, WHITE);
}

void state_moonlight_cleanup(GameContext *ctx) {
    StopMusicStream(playlist[BGM_FAST_DANGER]);
    cosched_finish(&ctx->sched);
    free(ctx->pool);
}

GameState state_moonlight = {state_moonlight_init, state_moonlight_update, state_moonlight_draw, state_moonlight_cleanup};
