#pragma once

#include "bullet.h"
#include "coroutine/tasks.h"
#include "cotask.h"
#include "hud.h"
#include "moonlight_bg.h"
#include "obj.h"
#include "player.h"
#include "macro.h"
#include "game_state.h"
#include "pool.h"
#include <raylib.h>

DECLARE_EXTERN_TASK(phase_timer, { CoEvent *event; int duration; });

DECLARE_EXTERN_TASK(obj_GoTo, {Pool *pool; Entity objId; float x; float y; float speed;});

DECLARE_EXTERN_TASK(play_anim_once, {Pool *pool; float x; float y; int sprite_id; });

DECLARE_EXTERN_TASK(boss_orb_effect, {Pool *pool; Entity boss; });
DECLARE_EXTERN_TASK(boss_pentagram_effect, {Pool *pool; Entity boss; });
DECLARE_EXTERN_TASK(boss_particle_spawner, {Pool *pool; Entity boss; });
DECLARE_EXTERN_TASK(boss_distortion_effect, {Pool *pool; Entity boss; Vector2 *lens_center; float *lens_radius; float *lens_strength;});

DECLARE_EXTERN_TASK(orb_explosion, {Pool *pool; float x; float y;});

DECLARE_EXTERN_TASK(spellcard_bg_anim, {Pool *pool; int duration; });
DECLARE_EXTERN_TASK(start_spellcard_sequence, {Pool *pool; Entity boss; const char* spell_name; int duration; });

#define RUN_SPELLCARD(pool, boss, spell, var, spell_name, life) \
    obj_SetMaxlife(pool, boss, life); \
    obj_SetLife(pool, boss, life); \
    INVOKE_SUBTASK(start_spellcard_sequence, pool, boss, spell_name, 120); \
    WAIT(120); \
    CoTask* MACRO_CONCAT(task_, var) = spell; \
    BoxedTask MACRO_CONCAT(box_, var) = cotask_box(MACRO_CONCAT(task_, var)); \
    \
    while(!obj_IsDead(pool, boss)) { \
        YIELD; \
    } \
\
    HUD_clear_spellcard(); \
    CANCEL_TASK(MACRO_CONCAT(box_, var)); \
    Bullet_clear_bullets(pool); \
    moonlight_bg_set_mode(false);

#define RUN_NONSPELL(pool, boss, nonspell, var, life) \
    moonlight_bg_set_mode(false); \
    obj_SetMaxlife(pool, boss, life); \
    obj_SetLife(pool, boss, life); \
    obj_AddFlag(pool, boss, FLAG_INVINCIBLE); \
    WAIT(120); \
    obj_RemoveFlag(pool, boss, FLAG_INVINCIBLE); \
    CoTask* MACRO_CONCAT(task_, var) = nonspell; \
    BoxedTask MACRO_CONCAT(box_, var) = cotask_box(MACRO_CONCAT(task_, var)); \
    while(!obj_IsDead(pool, boss)) { \
        YIELD; \
    } \
    CANCEL_TASK(MACRO_CONCAT(box_, var)); \
    Bullet_clear_bullets(pool);