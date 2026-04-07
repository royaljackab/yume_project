#include "content/stage/moonlight/moonlight.h"
#include "background.h"
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
#include "nonspells/nonspell2.h"
#include "spellcards/poincarre_recurrence.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static
int frames = 0;

void invoke_spellcard_background(Pool *p) {
    Entity base = Background_create(p, BG_MORIYA_FLOWERS, 0, 0);
    Entity overlay = Background_create(p, BG_MORIYA_CIRCLES, -1, 1);

    obj_SetScaleX(p, base, (float)PANEL_WIDTH/(float)sprites[BG_MORIYA_FLOWERS].srcRect.width);
    obj_SetScaleY(p, base, (float)PANEL_HEIGHT/(float)sprites[BG_MORIYA_FLOWERS].srcRect.height);

    obj_SetAlpha(p, overlay, 128);
    obj_SetRenderPriority(p, overlay, RENDER_PRIO_BG + 1);
    obj_SetColor(p, overlay, YELLOW.r, YELLOW.g, YELLOW.b);
    obj_SetScale(p, overlay, 3, 3);
}

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

    INVOKE_SUBTASK(boss_particle_spawner, ARGS.ctx->pool, boss);
    INVOKE_SUBTASK(obj_GoTo, ARGS.ctx->pool, boss, 500, 200, 5);

    WAIT(120);
    INVOKE_SUBTASK(boss_pentagram_effect, ARGS.ctx->pool, boss);
    WAIT(80);
    
    invoke_spellcard_background(ARGS.ctx->pool);
    INVOKE_SUBTASK(movement, ARGS.ctx, boss);



    obj_SetMaxlife(ARGS.ctx->pool, boss, 500);
    obj_SetLife(ARGS.ctx->pool, boss, 500);

    INVOKE_SUBTASK(obj_GoTo, ARGS.ctx->pool, boss, 500, 400, 5);
    WAIT(60);

    CoTask *attack_1 = INVOKE_SUBTASK(moriya_nonspell_1, ARGS.ctx->pool, boss);
    BoxedTask attack_1_box = cotask_box(attack_1);

    while(!obj_IsDead(ARGS.ctx->pool, boss)) {
        YIELD;
    }
    CANCEL_TASK(attack_1_box);
    Bullet_clear_bullets(ARGS.ctx->pool);

    obj_SetMaxlife(ARGS.ctx->pool, boss, 500);
    obj_SetLife(ARGS.ctx->pool, boss, 500);
    
    INVOKE_SUBTASK(obj_GoTo, ARGS.ctx->pool, boss, 500, 400, 5);
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

    INVOKE_SUBTASK(obj_GoTo, ARGS.ctx->pool, boss, 500, 400, 5);
    WAIT(60);

    CoTask *nonspell_2 = INVOKE_SUBTASK(moriya_nonspell_2, ARGS.ctx->pool, boss);
    BoxedTask nonspell_2_box = cotask_box(nonspell_2);

    while(!obj_IsDead(ARGS.ctx->pool, boss)) {
        YIELD;
    }
    CANCEL_TASK(nonspell_2_box);
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
    Background_update_all(ctx->pool);

    frames++;
}

void state_moonlight_draw(GameContext *ctx) {
    ClearBackground(BLACK);

    BeginScissorMode(PANEL_LEFT, PANEL_UP, PANEL_WIDTH, PANEL_HEIGHT);
    Sprite_draw_all(ctx->pool);
    draw_all_loose_lasers(&ctx->pool->looseLaser,&ctx->pool->position); 
    straight_lasers_draw_all(&ctx->pool->straightLaser,&ctx->pool->position,&ctx->pool->sprite); 

    //par sécurité: mettez plutôt la boss_bar après les sprites, peut-être que ça cause des problèmes
    bossbar_draw_all(ctx->pool);
    EndScissorMode();

    HUD_draw(ctx, "Stage 1 - Moonlight");
}

void state_moonlight_cleanup(GameContext *ctx) {
    StopMusicStream(playlist[BGM_FAST_DANGER]);
    cosched_finish(&ctx->sched);
    free(ctx->pool);
}

GameState state_moonlight = {state_moonlight_init, state_moonlight_update, state_moonlight_draw, state_moonlight_cleanup};