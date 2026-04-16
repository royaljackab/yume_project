#include "content/stage/moonlight/moonlight.h"
#include "background.h"
#include "bullet.h"
#include "common.h"
#include "coroutine/cosched.h"
#include "coroutine/cotask.h"
#include "ecs.h"
#include "content/assets.h"
#include "enemy.h"
#include "life.h"
#include "game_state.h"
#include "obj.h"
#include "physics.h"
#include "player.h"
#include "pool.h"
#include "score.h"
#include "screen.h"
#include "core/coroutine/tasks.h"
#include "common_task.h"
#include "sprite.h"
#include "systems/hud.h"
#include "boss.h"

#include "nonspells/nonspell1.h"
#include "nonspells/nonspell2.h"
#include "nonspells/nonspell3.h"
#include "spellcards/poincarre_recurrence.h"
#include "spellcards/brouwer_fixed_point.h"
#include "spellcards/axiom_of_choice.h"

#include "moonlight_bg.h"
#include "systems/screen_effects.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// Variables distorsions
static RenderTexture2D screen_target;
static Shader lens_shader;
static int center_loc, radius_loc, strength_loc;

static Vector2 lens_center = {0,0};
static float lens_radius = 0;
static float lens_strength = 0;

// Variables invert
static RenderTexture2D game_target;
static Shader invert_shader;
static int inv_centers_loc, inv_radius_loc;

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
    Pool *p = ARGS.ctx->pool;
    
    // Définition des zones de sécurité
    // limit_up : On ajoute une marge pour ne pas que le boss soit trop collé au haut
    float limit_up = PANEL_UP + 60.0f; 
    // limit_down : On garde ta logique de 40% de la hauteur du panel
    float limit_down = PANEL_DOWN * 0.4f;

    while(true) {
        float player_x = Player_GetX(p);
        float boss_x = obj_GetX(p, ARGS.boss);
        float boss_y = obj_GetY(p, ARGS.boss);

        float angleT = 0;

        if (boss_y < limit_up) {
            angleT = (float)GetRandomValue(45, 135);
        } 
        else if (boss_y > limit_down) {
            angleT = (float)GetRandomValue(225, 315);
        } 
        else {
            if (boss_x <= player_x) {
                angleT = (float)GetRandomValue(-45, 45);
            } else {
                angleT = (float)GetRandomValue(135, 225);
            }
        }

        obj_SetAngle(p, ARGS.boss, angleT);
        obj_SetSpeed(p, ARGS.boss, 2.0f);

        WAIT(60);
        
        obj_SetSpeed(p, ARGS.boss, 0.0f);
        WAIT(120);
    }
}

TASK(main_attack, {GameContext *ctx;}) {
    Pool *p = ARGS.ctx->pool;

    Entity boss = Boss_spawn(ARGS.ctx->pool, 10, 10, 200, 20, 200000, ENEMY_FAIRY_BIG_SUNFLOWER_IDLE);
    INVOKE_SUBTASK(obj_GoTo, ARGS.ctx->pool, boss, 500, 200, 5);
    WAIT(60);

    Boss_fight_begin(ARGS.ctx->pool, boss, &lens_center, &lens_radius, &lens_strength);
        
    INVOKE_SUBTASK(movement, ARGS.ctx, boss);

    invoke_spellcard_background(ARGS.ctx->pool);

    update_combo(&ARGS.ctx->score);
    RUN_NONSPELL(ARGS.ctx, boss,
        INVOKE_SUBTASK(moriya_nonspell_1, ARGS.ctx->pool, boss), 7000);
    
    INVOKE_SUBTASK(obj_GoTo, ARGS.ctx->pool, boss, 500, 200, 5);

    update_combo(&ARGS.ctx->score);
    RUN_SPELLCARD(ARGS.ctx, boss, 
        INVOKE_SUBTASK(poincarre_recurrence, ARGS.ctx->pool, boss, 10, 3.5, 100),
        "Theorem - Poincare Recurrence", 10000);
    
    update_combo(&ARGS.ctx->score);
    RUN_NONSPELL(ARGS.ctx, boss, 
        INVOKE_SUBTASK(moriya_nonspell_2, ARGS.ctx->pool, boss), 7000);

    INVOKE_SUBTASK(obj_GoTo, ARGS.ctx->pool, boss, 500, 200, 5);
    
    update_combo(&ARGS.ctx->score);
    RUN_SPELLCARD(ARGS.ctx, boss, 
        INVOKE_SUBTASK(brouwer_fixed_point, ARGS.ctx->pool, boss), "Theorem - Brouwer's fixed point", 10000);

    INVOKE_SUBTASK(obj_GoTo, ARGS.ctx->pool, boss, 500, 200, 5);

    INVOKE_SUBTASK(obj_GoTo, ARGS.ctx->pool, boss, 500, 200, 5);
    
    update_combo(&ARGS.ctx->score);
    RUN_NONSPELL(ARGS.ctx, boss, 
        INVOKE_SUBTASK(moriya_nonspell_3, ARGS.ctx->pool, boss), 7000);
    
    INVOKE_SUBTASK(obj_GoTo, ARGS.ctx->pool, boss, 500, 200, 5);
    
    update_combo(&ARGS.ctx->score);
    RUN_SPELLCARD(ARGS.ctx, boss, 
        INVOKE_SUBTASK(axiom_of_choice, p, boss), "ZFC - Axiom of Choice", 10000);
    
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
    PlayMusicStream(playlist[BGM_MORIYA_THEME]);

    FontsLoad();

    pool_init(ctx->pool);
    score_system_init(&ctx->score);

    cosched_init(&ctx->sched, ctx->pool);

    Player_start(ctx, TEST_PLAYER, DEFAULT_PATTERN);

    SCHED_INVOKE_TASK(&ctx->sched, main_attack, ctx);

    // Pour effet de distortion
    screen_target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    lens_shader = LoadShader(0, "Assets/Shaders/lens.fs");

    center_loc = GetShaderLocation(lens_shader, "center");
    radius_loc = GetShaderLocation(lens_shader, "radius");
    strength_loc = GetShaderLocation(lens_shader, "strength");

    // Pour effet d'inversion
    game_target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    invert_shader = LoadShader(0, "Assets/Shaders/invert.fs");

    inv_centers_loc = GetShaderLocation(invert_shader, "centers");
    inv_radius_loc = GetShaderLocation(invert_shader, "radius");
    invert_radius = 0;

    moonlight_bg_init();
}

void state_moonlight_update(GameContext *ctx) {
    /* Vérifier le game over */
    Entity player = ctx->pool->player.entity_lookup[0];
    Life *player_life = Life_get(&ctx->pool->life, player);
    if (player_life && player_life->life <= 0) {
        gamestate_change_state(ctx, STATE_GAME_OVER);
        return;
    }

    moonlight_bg_update(frames);

    cosched_run_tasks(&ctx->sched);

    Player_update(ctx);
    Physics_update_all(ctx->pool);
    loose_lasers_update_all(ctx->pool); 
    straight_lasers_update_all(ctx->pool);
    Condensation_update_all(ctx->pool);
    Owner_update(ctx->pool);
    
    Life_update_all(ctx, &ctx->score);
    Enemy_update_all(ctx->pool, &ctx->score);
    Background_update_all(ctx->pool);

    //en dernier par sécurité
    pool_kill_convicts(ctx->pool);
    frames++;
}

void state_moonlight_draw(GameContext *ctx) {
    // =======================================================
    // ETAPE 1 : PREPARER LE DECOR (Sur Toile 1 : screen_target)
    // =======================================================
    BeginTextureMode(screen_target);
        ClearBackground(BLANK); 
        BeginScissorMode(PANEL_LEFT, PANEL_UP, PANEL_WIDTH, PANEL_HEIGHT);
            moonlight_bg_draw(ctx);
        EndScissorMode();
    EndTextureMode();

    // =======================================================
    // ETAPE 2 : DESSINER TOUT LE JEU (Sur Toile 2 : game_target)
    // =======================================================
    // On dessine le Fond déformé + les Sprites sur une seule image
    BeginTextureMode(game_target);
        ClearBackground(BLANK);
        
        BeginShaderMode(lens_shader);
            SetShaderValue(lens_shader, center_loc, &lens_center, SHADER_UNIFORM_VEC2);
            SetShaderValue(lens_shader, radius_loc, &lens_radius, SHADER_UNIFORM_FLOAT);
            SetShaderValue(lens_shader, strength_loc, &lens_strength, SHADER_UNIFORM_FLOAT);
            Rectangle sourceRec1 = { 0.0f, 0.0f, (float)screen_target.texture.width, -(float)screen_target.texture.height };
            DrawTextureRec(screen_target.texture, sourceRec1, (Vector2){ 0, 0 }, WHITE);
        EndShaderMode();
        
        Sprite_draw_range(ctx->pool, 0, 100);
        draw_all_loose_lasers(&ctx->pool->looseLaser, &ctx->pool->position); 
        straight_lasers_draw_all(&ctx->pool->straightLaser, &ctx->pool->position, &ctx->pool->sprite); 
    EndTextureMode();

    // =======================================================
    // ETAPE 3 : DESSIN FINAL SUR L'ÉCRAN PHYSIQUE
    // =======================================================
    ClearBackground(BLANK); 
    HUD_draw_background();
    DrawRectangle(PANEL_LEFT, PANEL_UP, PANEL_WIDTH, PANEL_HEIGHT, BLACK); // Le cache pour les couleurs

    // On applique l'inversion sur l'intégralité du jeu !
    BeginScissorMode(PANEL_LEFT, PANEL_UP, PANEL_WIDTH, PANEL_HEIGHT);
        BeginShaderMode(invert_shader);
            
            // On envoie le tableau de 5 vecteurs et le rayon à la carte graphique
            SetShaderValueV(invert_shader, inv_centers_loc, invert_centers, SHADER_UNIFORM_VEC2, 4);
            SetShaderValue(invert_shader, inv_radius_loc, &invert_radius, SHADER_UNIFORM_FLOAT);
            
            Rectangle sourceRec2 = { 0.0f, 0.0f, (float)game_target.texture.width, -(float)game_target.texture.height };
            DrawTextureRec(game_target.texture, sourceRec2, (Vector2){ 0, 0 }, WHITE);
            
        EndShaderMode();
    EndScissorMode();

    bossbar_draw_all(ctx->pool);
    // ETAPE 4 : LE HUD EN DERNIER
    HUD_draw_foreground(ctx, "Stage 1 - Moonlight");
}

void state_moonlight_cleanup(GameContext *ctx) {
    StopMusicStream(playlist[BGM_FAST_DANGER]);
    moonlight_bg_set_mode(false);
    cosched_finish(&ctx->sched);
    free(ctx->pool);
    UnloadRenderTexture(screen_target);
    UnloadShader(lens_shader);
    UnloadShader(invert_shader);
}



GameState state_moonlight = {state_moonlight_init, state_moonlight_update, state_moonlight_draw, state_moonlight_cleanup};