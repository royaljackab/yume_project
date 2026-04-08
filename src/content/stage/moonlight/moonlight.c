#include "content/stage/moonlight/moonlight.h"
#include "background.h"
#include "bullet.h"
#include "common.h"
#include "coroutine/cosched.h"
#include "cotask.h"
#include "ecs.h"
#include "content/assets.h"
#include "enemy.h"
#include "life.h"
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
#include "spellcards/brouwer_fixed_point.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static RenderTexture2D screen_target;
static Shader lens_shader;
static int center_loc, radius_loc, strength_loc;

static Vector2 lens_center = {0,0};
static float lens_radius = 0;
static float lens_strength = 0;


static
int frames = 0;

void invoke_spellcard_background(Pool *p) {
    Entity base = Background_create(p, BG_SC_TORII, 0, 0);
    Entity overlay = Background_create(p, BG_SC_OV_MATH, -0.5, 0.5);


    obj_SetScaleX(p, base, (float)PANEL_WIDTH/(float)sprites[BG_SC_TORII].srcRect.width);
    obj_SetScaleY(p, base, (float)PANEL_HEIGHT/(float)sprites[BG_SC_TORII].srcRect.height);

    obj_SetAlpha(p, overlay, 128);
    obj_SetRenderPriority(p, overlay, RENDER_PRIO_BG + 1);
    // obj_SetColor(p, overlay, YELLOW.r, YELLOW.g, YELLOW.b);
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
    INVOKE_SUBTASK(boss_distortion_effect, ARGS.ctx->pool, boss, &lens_center, &lens_radius, &lens_strength);
    INVOKE_SUBTASK(boss_orb_effect, ARGS.ctx->pool, boss);
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

    INVOKE_SUBTASK(obj_GoTo, ARGS.ctx->pool, boss, 500, 400, 5);
    WAIT(60);

    CoTask *nonspell_2 = INVOKE_SUBTASK(moriya_nonspell_2, ARGS.ctx->pool, boss);
    BoxedTask nonspell_2_box = cotask_box(nonspell_2);

    while(!obj_IsDead(ARGS.ctx->pool, boss)) {
        YIELD;
    }
    CANCEL_TASK(nonspell_2_box);
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


    gamestate_change_state(ARGS.ctx, STATE_VICTORY);
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

    // Pour effet de distortion
    screen_target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    lens_shader = LoadShader(0, "../Assets/Shaders/lens.fs");

    center_loc = GetShaderLocation(lens_shader, "center");
    radius_loc = GetShaderLocation(lens_shader, "radius");
    strength_loc = GetShaderLocation(lens_shader, "strength");
}

void state_moonlight_update(GameContext *ctx) {
    /* Vérifier le game over */
    Entity player = ctx->pool->player.entity_lookup[0];
    Life *player_life = Life_get(&ctx->pool->life, player);
    if (player_life && player_life->life <= 0) {
        gamestate_change_state(ctx, STATE_GAME_OVER);
        return;
    }

    cosched_run_tasks(&ctx->sched);

    Player_update(ctx);
    Physics_update_all(ctx->pool);
    loose_lasers_update_all(ctx->pool); 
    straight_lasers_update_all(ctx->pool);
    Condensation_update_all(ctx->pool);
    Owner_update(ctx->pool);
    
    Life_update_all(ctx->pool, &ctx->score);
    Enemy_update_all(ctx->pool, &ctx->score);
    Background_update_all(ctx->pool);

    //en dernier par sécurité
    pool_kill_convicts(ctx->pool);
    frames++;
}

void state_moonlight_draw(GameContext *ctx) {
    
    // DECORS
    BeginTextureMode(screen_target);
        ClearBackground(BLANK); 
        BeginScissorMode(PANEL_LEFT, PANEL_UP, PANEL_WIDTH, PANEL_HEIGHT);
            Sprite_draw_range(ctx->pool, -50, -1); 
        EndScissorMode();
    EndTextureMode();


    // FOND UI
    ClearBackground(BLACK); 
    HUD_draw_background(); 

    DrawRectangle(PANEL_LEFT, PANEL_UP, PANEL_WIDTH, PANEL_HEIGHT, BLACK);


    // JEU ET SHADER
    BeginScissorMode(PANEL_LEFT, PANEL_UP, PANEL_WIDTH, PANEL_HEIGHT);

        // Distorsion
        BeginShaderMode(lens_shader);
            SetShaderValue(lens_shader, center_loc, &lens_center, SHADER_UNIFORM_VEC2);
            SetShaderValue(lens_shader, radius_loc, &lens_radius, SHADER_UNIFORM_FLOAT);
            SetShaderValue(lens_shader, strength_loc, &lens_strength, SHADER_UNIFORM_FLOAT);
            
            Rectangle sourceRec = { 0.0f, 0.0f, (float)screen_target.texture.width, -(float)screen_target.texture.height };
            DrawTextureRec(screen_target.texture, sourceRec, (Vector2){ 0, 0 }, WHITE);
        EndShaderMode();

        // Jeu
        Sprite_draw_range(ctx->pool, 0, 100);
        draw_all_loose_lasers(&ctx->pool->looseLaser, &ctx->pool->position); 
        straight_lasers_draw_all(&ctx->pool->straightLaser, &ctx->pool->position, &ctx->pool->sprite); 
        bossbar_draw_all(ctx->pool);

    EndScissorMode();


    // HUD textes
    HUD_draw_foreground(ctx, "Stage 1 - Moonlight");
}

void state_moonlight_cleanup(GameContext *ctx) {
    StopMusicStream(playlist[BGM_FAST_DANGER]);
    cosched_finish(&ctx->sched);
    free(ctx->pool);
    UnloadRenderTexture(screen_target);
    UnloadShader(lens_shader);
}



GameState state_moonlight = {state_moonlight_init, state_moonlight_update, state_moonlight_draw, state_moonlight_cleanup};