#include "content/stage/moonlight/moonlight.h"
#include "common.h"
#include "coroutine/cosched.h"
#include "ecs.h"
#include "content/assets.h"
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
#include "systems/hud.h"
#include "components/enemy.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static
void fireRing(GameContext *ctx, float x, float y, int nb_ring, float angleT) {
    for (int i=0; i < nb_ring; i++) {
        Bullet_enemy_spawn(ctx->pool, x, y, 5, angleT, BALL_M_BLACK);
        angleT += 360.0 / nb_ring;
    }
}

TASK(pulse_ring, {GameContext *ctx; float x; float y;}) {
    const int num_shots = 5;
    const int num_projs = 12;

    Entity player_ent = Player_get_entity(&ARGS.ctx->pool->player, 0);

    Position *player_pos = Position_get(&ARGS.ctx->pool->position, player_ent);
    float aim_angle = atan2f(player_pos->pos.y - ARGS.y, player_pos->pos.x - ARGS.x) * RAD2DEG;

    for (int shot = 0; shot < num_shots; shot++) {
        float final_speed = 3.0 + shot / 3.0;
        float boost = shot * 0.7;

        float init_speed = final_speed + boost;

        for (int i=0; i < num_projs; i++) {
            float current_angle = aim_angle + (i * 360.0 / num_projs);

            Entity bullet = Bullet_enemy_spawn(ARGS.ctx->pool, ARGS.x, ARGS.y, init_speed, current_angle, MISSILE_LIGHT_RED);
            Physics *phy = Physics_get(&ARGS.ctx->pool->physics, bullet);
            Physics_set_accel(phy, -0.6);
            Physics_set_minSpd(phy, final_speed);
        }

        WAIT(2);
    }
}

TASK(crystal_shower, {GameContext *ctx; float x; float y;}) {
    for (int t=0, i=0; t<60; i++) {
        float speed0 = 10;
        float speed1 = 4;

        float angle_offset = GetRandomValue(-150, 90);
        
        float sign = 1 - 2 *(i & 1);

        float spawn_x = ARGS.x + (30 * sign);
        float spawn_y = ARGS.y - 42;

        float start_angle = 270.0 + angle_offset;

        Entity bullet = Bullet_enemy_spawn(ARGS.ctx->pool, spawn_x, spawn_y, speed0, start_angle, BALL_M_BLACK);
        Physics *phy = Physics_get(&ARGS.ctx->pool->physics, bullet);
        Physics_set_force(phy, (Vector2){0, 0.35});
        Physics_set_maxSpd(phy, speed0);

        t += WAIT(1);
    }
}   


TASK(laser_task,  {GameContext *ctx; int amount; int x; int y; int length;}) {
    while(true){
            for(int i = 0; i < ARGS.amount;i++){
            Entity id = straight_laser_create(ARGS.ctx->pool, ARGS.x, ARGS.y, i * 360 / ARGS.amount, ARGS.length, 40, 80, 40, 100, LASER_LIGHT_CYAN);
            Physics_set_angVel(Physics_get(&ARGS.ctx->pool->physics,id),0.3);
        }
        WAIT(300);
    }
}



TASK(moonlight_task, { GameContext *ctx; }) {
    INVOKE_SUBTASK(laser_task, ARGS.ctx, 3, 500, 500 ,700);
    Enemy_spawn(ARGS.ctx->pool, 700,800, 0, 0, 10, 5, 0, ENEMY_FAIRY_PURPLE_IDLE);
    Enemy_spawn(ARGS.ctx->pool, 600,800, 0, 0, 10, 5, 0, ENEMY_FAIRY_BLACK_BLONDE_IDLE);
    Enemy_spawn(ARGS.ctx->pool, 500,800, 0, 0, 10, 5, 0, ENEMY_FAIRY_RED_IDLE);
    Enemy_spawn(ARGS.ctx->pool, 400,800, 0, 0, 10, 5, 0, ENEMY_FAIRY_BLUE_IDLE);
    Enemy_spawn(ARGS.ctx->pool, 300,800, 0, 0, 10, 5, 0, ENEMY_FAIRY_BIG_SUNFLOWER_IDLE);
    Enemy_spawn(ARGS.ctx->pool, 200,800, 0, 0, 10, 5, 0, ENEMY_FAIRY_BIG_EVIL_IDLE);
    Enemy_spawn(ARGS.ctx->pool, 100,800, 0, 0, 10, 5, 0, ENEMY_FAIRY_GREEN_IDLE);
    while (true) {
        WAIT(30);
        INVOKE_SUBTASK(pulse_ring, ARGS.ctx, 500, 500);
        
        // WAIT(10);
        // INVOKE_SUBTASK(crystal_shower, ARGS.ctx, 500, 500);
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

    Enemy_spawn(ctx->pool, 480, 200, 0, 0, 5, 20,
        ENEMY_TYPE_FAIRY, ENEMY_FAIRY_BLUE_IDLE);

    cosched_init(&ctx->sched, ctx->pool);

    SCHED_INVOKE_TASK(&ctx->sched, moonlight_task, ctx);
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