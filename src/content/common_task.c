#include "common_task.h"
#include "assets.h"
#include "coevent.h"
#include "common.h"
#include "screen.h"
#include "systems/obj.h"
#include "pool.h"
#include "sprite.h"
#include "tasks.h"
#include <math.h>
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

    obj_SetAngle(p, aura, -90 + GetRandomValue(-30, 30) / 3.0);
    obj_SetScale(p, aura, 1.5, 1.9);

    const int alpha = 200;

    obj_SetColor(p, aura, 200, 30, 30);
    obj_SetAlpha(p, aura, alpha);

    const int duration = 30;
    const float scale_grow_x = 0.1;
    const float scale_grow_y = 0.05;

    for (int i=0; i < duration; i++) {
        float boss_x = obj_GetX(p, ARGS.boss);
        float boss_y = obj_GetY(p, ARGS.boss);

        obj_SetPosition(p, aura, boss_x, boss_y);


        obj_SetScaleX(p, aura, obj_GetScaleX(p, aura) + scale_grow_x);
        obj_SetScaleY(p, aura, obj_GetScaleY(p, aura) + scale_grow_y);

        obj_SetAlpha(p, aura, alpha * (1.0 - (float)i/duration));

        WAIT(1);
    }

    pool_kill_entity(p, aura);
}

DEFINE_EXTERN_TASK(boss_particle_spawner) {
    TASK_BIND(ARGS.boss);

    while(true) {
        INVOKE_SUBTASK(boss_aura_particle, ARGS.pool, ARGS.boss);
        WAIT(5);
    }
}

DEFINE_EXTERN_TASK(boss_pentagram_effect) {
    Pool *p = ARGS.pool;

    TASK_BIND(ARGS.boss);

    Entity penta = pool_create_entity(p);
    Position pos = {{0,0}, 0};
    Position_add(&p->position, penta, pos);

    Sprite_add(&p->sprite, penta, sprites[BOSS_PENTAGRAM]);
    obj_SetScale(p, penta, 0, 0);
    obj_SetAlpha(p, penta, 200);
    
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

DEFINE_EXTERN_TASK(boss_distortion_effect) {
    Pool *p = ARGS.pool;

    TASK_BIND(ARGS.boss);

    *(ARGS.lens_radius) = 300.0f; 
    float max_strength = 5.0f;    
    int duration = 60;            
    
    // Latence
    float follow_speed = 0.07f; 

    // Initialisation immédiate au spawn pour éviter que la bulle 
    // ne traverse tout l'écran depuis (0,0) à la frame 1 !
    Position *init_pos = Position_get(&p->position, ARGS.boss);
    if (init_pos) {
        ARGS.lens_center->x = init_pos->pos.x;
        ARGS.lens_center->y = GetScreenHeight() - init_pos->pos.y;
    }

    for(int i = 0; i < duration; i++) {
        if (obj_IsDead(p, ARGS.boss)) {
            *(ARGS.lens_strength) = 0.0f;
            return; 
        }

        Position *pos = Position_get(&p->position, ARGS.boss);
        if (pos) {
            // 1. On calcule la vraie cible (position du boss inversée pour le shader)
            Vector2 target;
            target.x = pos->pos.x;
            target.y = GetScreenHeight() - pos->pos.y; 
            
            // 2. La magie élastique : on glisse doucement vers la cible !
            *ARGS.lens_center = Vector2Lerp(*ARGS.lens_center, target, follow_speed);
        }
        
        float t = (float)i / duration; 
        *(ARGS.lens_strength) = sinf(t * (PI / 2.0f)) * max_strength; 
        
        WAIT(1);
    }
    
    *(ARGS.lens_strength) = max_strength;

    while (true) {
        if (obj_IsDead(p, ARGS.boss)) {
            break; 
        }

        Position *pos = Position_get(&p->position, ARGS.boss);
        if (pos) {
            Vector2 target;
            target.x = pos->pos.x;
            target.y = GetScreenHeight() - pos->pos.y; 
            
            // On continue de glisser avec la latence
            *ARGS.lens_center = Vector2Lerp(*ARGS.lens_center, target, follow_speed);
        }

        WAIT(1);
    }
    
    *(ARGS.lens_strength) = 0.0f;
}

DEFINE_EXTERN_TASK(boss_orb_effect) {
    Pool *p = ARGS.pool;

    TASK_BIND(ARGS.boss);

    Entity orb = pool_create_entity(p);
    Position pos= {{0,0},0};
    Position_add(&p->position, orb, pos);

    Sprite_add(&p->sprite, orb, sprites[BOSS_AURA_ORB]);
    obj_SetAlpha(p, orb, 50);
    obj_SetColor(p, orb, 0, 153, 204);

    const float target_scale = 2;
    const int grow_duration = 60;
    const float follow_speed = 0.07; // La meme que pour la boule

    // Phase 1 : IL devient GRAND
    for (int i=0; i < grow_duration; i++) {
        if(obj_IsDead(p, ARGS.boss)) {
            pool_kill_entity(p, orb);
            return;
        }

        float boss_x = obj_GetX(p, ARGS.boss);
        float boss_y = obj_GetY(p, ARGS.boss);

        obj_SetPosition(p, orb, boss_x, boss_y);

        float t = (float)i / grow_duration;

        float grandition = 1 - powf(1-t, 5);

        obj_SetScale(p, orb, target_scale * grandition, target_scale * grandition);

        YIELD;
    }

    // Ca c ma go nan, tatata ca c ma go
    // Stabilise puis tu rentabilises
    obj_SetScale(p, orb, target_scale, target_scale);

    // Dans le carré pas de coeur de pierre, j'ai le tournis le tournis
    while(true) {
        if(obj_IsDead(p, ARGS.boss)) {
            pool_kill_entity(p, orb);
            return;
        }

        float boss_x = obj_GetX(p, ARGS.boss);
        float boss_y = obj_GetY(p, ARGS.boss);

        Vector2 larp = Vector2Lerp( Position_get(&p->position,  orb)->pos, (Vector2){boss_x, boss_y}, follow_speed);
        obj_SetPosition(p, orb, larp.x, larp.y);

        YIELD;
    }
}

DEFINE_EXTERN_TASK(orb_explosion) {
    Pool *p = ARGS.pool;
    Entity orb = pool_create_entity(p);

    Position pos = {{ARGS.x,ARGS.y}, 0};
    Position_add(&p->position, orb, pos);

    Sprite_add(&p->sprite, orb, sprites[HIT_ORB]);
    obj_SetColor(p, orb, 255, 150, 150);

    int duration = 50;
    float target_scale = 5;
    for (int i=0; i < duration; ++i) {
        float t = (float)i / duration;

        obj_SetScale(p, orb, t * target_scale, t * target_scale);
        obj_SetAlpha(p, orb, 255 * (1 - t));

        YIELD;
    }

    pool_kill_entity(p, orb);
}

DEFINE_EXTERN_TASK(spellcard_bg_anim) {
    Pool *p = ARGS.pool;
    int duration = ARGS.duration;

    const int num_lines = 30;
    const int sprites_per_lines = 16;
    const float spacing_line = 70;
    const float spacing_sprite = 100;
    float line_length = sprites_per_lines * spacing_sprite;

    const float angle = 135 * DEG2RAD;
    const float speed = 2;

    float dir_x = cosf(angle);
    float dir_y = sinf(angle);
    float perp_x = -sinf(angle);
    float perp_y = cosf(angle);

    float cx = PANEL_RIGHT / 2.0 ;
    float cy = PANEL_DOWN / 2.0;

    Entity bg_sprites[30 * 16];

    // Création des entités
    for (int i = 0; i < num_lines; ++i) {
        for (int j=0; j < sprites_per_lines; ++j) {
            Entity e = pool_create_entity(p);
            bg_sprites[i * sprites_per_lines + j ] = e;

            Position pos = {{0,0}, 0};
            Position_add(&p->position, e, pos);

            Sprite_add(&p->sprite, e, sprites[SPELL_CARD_ATTACK]);

            obj_SetAngle(p, e, angle * RAD2DEG);
            obj_SetAlpha(p, e, 120);
        }
    }

    // Animation
    for (int time = 0; time<duration; time++) {
        for (int i = 0; i < num_lines; ++i) {
            // On centre l'indice autour de 0
            int line_offset = i - (num_lines / 2);

            // paire -> droite, impaire -> gauche ou inverse jsp
            float current_speed = (i % 2 == 0) ? speed : -speed;
            float movement = time *current_speed;

            float line_center_x = cx + line_offset * spacing_line * perp_x;
            float line_center_y = cy + line_offset * spacing_line * perp_y;

            for (int j=0 ; j < sprites_per_lines; j++) {
                int sprite_offset = j - (sprites_per_lines / 2);
                Entity e = bg_sprites[i * sprites_per_lines + j ];

                float dist = sprite_offset * spacing_sprite + movement;

                dist = fmodf(dist + 1000 * line_length, line_length);
                dist -= line_length / 2;

                float final_x = line_center_x + dist * dir_x;
                float final_y = line_center_y + dist * dir_y;

                obj_SetPosition(p, e, final_x, final_y);
                obj_SetRenderPriority(p, e, 0);

                obj_SetAlpha(p, e, -cosf(time * 4 * PI / duration) * 100 + 100);
            }
        }

        YIELD;
    }

    for (int i=0; i < num_lines * sprites_per_lines; ++i) {
        pool_kill_entity(p, bg_sprites[i]);
    }
}