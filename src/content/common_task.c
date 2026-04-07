#include "common_task.h"
#include "assets.h"
#include "coevent.h"
#include "common.h"
#include "obj.h"
#include "pool.h"
#include "sprite.h"
#include "tasks.h"
#include <raylib.h>
#include <raymath.h>

DEFINE_EXTERN_TASK(phase_timer) {
    WAIT(ARGS.duration);

    coevent_signal(ARGS.event);
}

DEFINE_EXTERN_TASK(obj_GoTo) {
    TASK_BIND(ARGS.objId);

    while(true) {
        Vector2 pos = obj_GetPosition(ARGS.pool, ARGS.objId);
        Vector2 dest = (Vector2){ARGS.x, ARGS.y};

        if (Vector2Equals(pos, dest)) break;

        Vector2 intermid = Vector2MoveTowards(pos, dest, ARGS.speed);
        obj_SetPosition(ARGS.pool, ARGS.objId, intermid.x, intermid.y);

        YIELD;
    }
}

DEFINE_EXTERN_TASK(play_anim_once) {
    Entity anim_entity = pool_create_entity(ARGS.pool);

    Position pos = {.pos = {ARGS.x, ARGS.y}, .angle = 0};
    Position_add(&ARGS.pool->position, anim_entity, pos);

    Sprite_add(&ARGS.pool->sprite, anim_entity, sprites[ARGS.sprite_id]);

    Sprite *spr = Sprite_get(&ARGS.pool->sprite, anim_entity);
    int duration_frames = 1;

    if (spr && spr->isAnimated) {
        duration_frames = spr->animFrameCount * spr->animSpeed;
    }

    WAIT(duration_frames);

    pool_kill_entity(ARGS.pool, anim_entity);
}

TASK(boss_aura_particle, {Pool *pool; Entity boss;}) {
    Pool *p = ARGS.pool;

    Entity aura = pool_create_entity(p);
    Position pos = {{0,0}, 0};
    Position_add(&p->position, aura, pos);
    Sprite_add(&p->sprite, aura, sprites[BOSS_AURA_WAVES]);

    obj_SetAngle(p, aura, -90 + GetRandomValue(-5, 5) / 3.0);

    obj_SetColor(p, aura, 255, 30, 30);
    obj_SetAlpha(p, aura, 180);

    const int duration = 60;
    const float scale_grow_x = 0.04;
    const float scale_grow_y = 0.02;

    for (int i=0; i < 60; i++) {
        float boss_x = obj_GetX(p, ARGS.boss);
        float boss_y = obj_GetY(p, ARGS.boss);

        obj_SetPosition(p, aura, boss_x, boss_y);


        obj_SetScaleX(p, aura, obj_GetScaleX(p, aura) + scale_grow_x);
        obj_SetScaleY(p, aura, obj_GetScaleY(p, aura) + scale_grow_y);

        obj_SetAlpha(p, aura, 200 * (1.0 - (float)i/duration));

        WAIT(1);
    }

    pool_kill_entity(p, aura);
}

DEFINE_EXTERN_TASK(boss_particle_spawner) {
    TASK_BIND(ARGS.boss);

    while(true) {
        INVOKE_SUBTASK(boss_aura_particle, ARGS.pool, ARGS.boss);
        WAIT(15);
    }
}

DEFINE_EXTERN_TASK(boss_pentagram_effect) {
    Pool *p = ARGS.pool;

    Entity penta = pool_create_entity(p);
    Position pos = {{0,0}, 0};
    Position_add(&p->position, penta, pos);

    Sprite_add(&p->sprite, penta, sprites[BOSS_PENTAGRAM]);
    obj_SetScale(p, penta, 0, 0);
    
    const float target_scale = 4;
    const int grow_duration = 60;
    const float ang_vel = 5;
    const float pulse_freq = 0.1; // Vitesse du rebond
    const float pulse_amp = 0.15; 

    // Phase 1 : il grandit autour du boss
    for (int i=0; i < grow_duration; ++i) {
        float boss_x = obj_GetX(p, ARGS.boss);
        float boss_y = obj_GetY(p, ARGS.boss);
        obj_SetPosition(p, penta, boss_x, boss_y);

        // effet d'agrandissements, a tester plusieurs :
        // 4 : ease-out quint
        float t = (float)i / grow_duration;

        float grandition = 1 - powf(1-t, 5);

        obj_SetScale(p, penta, target_scale * grandition, target_scale * grandition);

        YIELD;
    }

    // Phase 2 : Stabilise puis tu rentabilises
    obj_SetScale(p, penta, target_scale, target_scale);


    // Phase 3 : Y a un cul derriere y a un cul devant, Y TOURNER ET TOURNER
    long frame = 0;
    while(true) {
        if (obj_IsDead(p, ARGS.boss)) {
            pool_kill_entity(p, penta);
            return;
        }

        float boss_x = obj_GetX(p, ARGS.boss);
        float boss_y = obj_GetY(p, ARGS.boss);
        obj_SetPosition(p, penta, boss_x, boss_y);
        obj_SetAngle(p, penta, obj_GetAngle(p, penta) + ang_vel);

        // mmmh ces pulsations... ça me rappelle la prepa :(
        float offset = sin((float)frame * pulse_freq) * pulse_amp;
        obj_SetScale(p, penta, target_scale + offset, target_scale + offset);

        frame++;
        YIELD;
    }
}