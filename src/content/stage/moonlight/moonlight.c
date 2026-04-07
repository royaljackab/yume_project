#include "content/stage/moonlight/moonlight.h"
#include "bullet.h"
#include "common.h"
#include "coroutine/cosched.h"
#include "cotask.h"
#include "ecs.h"
#include "content/assets.h"
#include "enemy.h"
#include "game_state.h"
#include "obj.h"
#include "physics.h"
#include "player.h"
#include "pool.h"
#include "screen.h"
#include "core/coroutine/tasks.h"
#include "common_task.h"
#include "systems/hud.h"
#include "boss.h"

#include "nonspells/nonspell1.h"
#include "spellcards/poincarre_recurrence.h"
#include "spellcards/brouwer_fixed_point.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static
int frames = 0;

// TASK(crystal_wall, {GameContext *ctx;}) {
//     const int num_crystals = 27;
//     float spacing = PANEL_WIDTH / (float)(num_crystals - 1);
//     float ofs = GetRandomValue(0, 5) - 1;

//     for (int i=0; i < 30; i++) {
//         for (int c = 0; c < num_crystals; ++c) {
//             Vector2 accel = {0, 0.02 + 0.01 * ((c%2) ? 1 : -1) * sin((c*3+frames) / 30.0)};
//             Entity bullet = Bullet_enemy_spawn(ARGS.ctx->pool, (ofs + c) * spacing + 10, PANEL_UP + 5, 0, 0, BULLET_RED);
//             float r = (c % 2) ? 100 : 255;
//             float g = (c % 2) ? 100 : 255;
//             float b = (c % 2) ? 200 : 255;
//             obj_SetColor(ARGS.ctx->pool, bullet, r, g, b);
//             obj_SetForce(ARGS.ctx->pool, bullet, 0, accel.y);
//         }

//         WAIT(10);
//     }
// }

// TASK(rotating_laser, { GameContext *ctx; }) {
//     Entity laser = straight_laser_create(ARGS.ctx->pool, 500, 500, 90, 1500, 30, 5, 5, 10000, LASER_GOLD);
//     TASK_BIND(laser);

//     obj_SetAngularSpeed(ARGS.ctx->pool, laser, 0.2);
// }

// TASK(frostbolt, {GameContext *ctx; Vector2 pos; float angle; float speed;}) {
//     Entity bullet = Bullet_enemy_spawn(ARGS.ctx->pool, ARGS.pos.x, ARGS.pos.y, ARGS.speed, ARGS.angle, ANIM_TEST);
//     TASK_BIND(bullet);

//     WAIT(300);
// }

TASK(movement, {GameContext *ctx; Entity boss; }) {
    while(true) {
        float player_x = Player_GetX(ARGS.ctx->pool);
        float boss_x = obj_GetX(ARGS.ctx->pool, ARGS.boss);

        float angleT = 0;

        if (boss_x <= player_x) {
            angleT = GetRandomValue(-45, 45);
        } else {
            angleT = GetRandomValue(135,215);
        }

        obj_SetAngle(ARGS.ctx->pool, ARGS.boss, angleT);
        obj_SetSpeed(ARGS.ctx->pool, ARGS.boss, 2);

        WAIT(60);
        obj_SetSpeed(ARGS.ctx->pool, ARGS.boss, 0);
        WAIT(120);
    }
}

TASK(main_attack, {GameContext *ctx;}) {
    Entity boss = Enemy_spawn(ARGS.ctx->pool, 20, 20, 0, 0, 200, 3, 1, ENEMY_FAIRY_BIG_SUNFLOWER_IDLE);
    obj_SetTag(ARGS.ctx->pool, boss, ENT_BOSS);
    TASK_BIND(boss);

    INVOKE_SUBTASK(obj_GoTo, ARGS.ctx->pool, boss, 500, 200, 5);

    WAIT(120);

    INVOKE_SUBTASK(movement, ARGS.ctx, boss);
    CoTask *attack_1 = INVOKE_SUBTASK(moriya_nonspell_1, ARGS.ctx->pool, boss);
    BoxedTask attack_1_box = cotask_box(attack_1);

    while(!obj_IsDead(ARGS.ctx->pool, boss)) {
        YIELD;
    }
    CANCEL_TASK(attack_1_box);
    Bullet_clear_bullets(ARGS.ctx->pool);

    obj_SetMaxlife(ARGS.ctx->pool, boss, 500);
    obj_SetLife(ARGS.ctx->pool, boss, 500);
    
    INVOKE_SUBTASK(obj_GoTo, ARGS.ctx->pool, boss, 500, 200, 5);
    WAIT(60);

    CoTask *spell_1 = INVOKE_SUBTASK(poincarre_recurrence, ARGS.ctx->pool, boss, 10, 3.5, 100);
    BoxedTask spell_1_box = cotask_box(spell_1);

    while (!obj_IsDead(ARGS.ctx->pool, boss)) {
        YIELD;
    }
    CANCEL_TASK(spell_1_box);
    Bullet_clear_bullets(ARGS.ctx->pool);

    obj_SetMaxlife(ARGS.ctx->pool, boss, 500);
    obj_SetLife(ARGS.ctx->pool, boss, 500);

    INVOKE_SUBTASK(obj_GoTo, ARGS.ctx->pool, boss, 500, 200, 5);
    WAIT(60);

    CoTask *spell_2 = INVOKE_SUBTASK(brouwer_fixed_point, ARGS.ctx->pool, boss, 10, 3.5, 100);
    BoxedTask spell_2_box = cotask_box(spell_2);

    while (!obj_IsDead(ARGS.ctx->pool, boss)) {
        YIELD;
    }
    CANCEL_TASK(spell_2_box);
    Bullet_clear_bullets(ARGS.ctx->pool);


    STALL;
}

void state_moonlight_init(GameContext *ctx) {
    ctx->pool = malloc(sizeof(Pool));
    if (!ctx->pool) {
        printf("FATAL ERROR: Moonlight pool allocation failed\n");
        return;
    }
    
    // Change the beat bruv
    StopMusicStream(playlist[BGM_WAITING]);
    SetMusicVolume(playlist[BGM_FAST_DANGER], ctx->volume_bgm);
    PlayMusicStream(playlist[BGM_MORIYA_THEME]);

    pool_init(ctx->pool);
    score_system_init(&ctx->score);
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
    Condensation_update_all(ctx->pool);
    Owner_update(ctx->pool);
    pool_kill_convicts(ctx->pool);
    Enemy_update_all(ctx->pool, &ctx->score);

    frames++;
}

void state_moonlight_draw(GameContext *ctx) {
    ClearBackground(BLACK);

    Sprite_draw_all(ctx->pool);
    draw_all_loose_lasers(&ctx->pool->looseLaser,&ctx->pool->position); 
    straight_lasers_draw_all(&ctx->pool->straightLaser,&ctx->pool->position,&ctx->pool->sprite); 

    //par sécurité: mettez plutôt la boss_bar après les sprites, peut-être que ça cause des problèmes
    bossbar_draw_all(ctx->pool);

    HUD_draw(ctx, "Stage 1 - Moonlight");
    DrawText("coucou", 30, 30, 50, WHITE);
}

void state_moonlight_cleanup(GameContext *ctx) {
    StopMusicStream(playlist[BGM_FAST_DANGER]);
    cosched_finish(&ctx->sched);
    free(ctx->pool);
}



GameState state_moonlight = {state_moonlight_init, state_moonlight_update, state_moonlight_draw, state_moonlight_cleanup};