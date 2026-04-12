#include "nonspells/nonspell3.h"
#include "assets.h"
#include "bullet.h"
#include "obj.h"
#include "tasks.h"
#include <raylib.h>

TASK(bullet_slow_then_turn, {Pool *pool; float x; float y; float start_speed; float speed; float angle; float angle_bullet;}) {
    Entity bullet = Bullet_enemy_spawn_delayed(ARGS.pool, ARGS.x, ARGS.y, ARGS.start_speed, ARGS.angle, PETAL_BLUE, 5);
    obj_SetMinSpd(ARGS.pool, bullet, 0);
    const float accel = -0.28;
    obj_SetAcceleration(ARGS.pool, bullet, accel);
    TASK_BIND(bullet);

    const int wait_time = 80;

    while(true) {
        if (obj_GetSpeed(ARGS.pool, bullet) <= 0) {
            WAIT(wait_time);
            obj_SetAngle(ARGS.pool, bullet, ARGS.angle + ARGS.angle_bullet);
            obj_SetSpeed(ARGS.pool, bullet, ARGS.speed);
            obj_SetAcceleration(ARGS.pool, bullet, 0);
        }

        YIELD;
    }
}

TASK(spiral_of_turn_bullets, {Pool *pool; Entity boss; int nb_bullet_per_ring; int pause_between_rings; float rotation_spiral;}) {
    Pool *p = ARGS.pool;

    const float start_speed = 15;
    const float speed = 2.2;
    const float angle_bullet = 50;

    float angle1 = GetRandomValue(0, 360);
    float angle2 = angle1;
    while(true) {

        float boss_x = obj_GetX(p, ARGS.boss);
        float boss_y = obj_GetY(p, ARGS.boss);

        for (int i=0; i<ARGS.nb_bullet_per_ring; ++i) {
            INVOKE_TASK(bullet_slow_then_turn, p, boss_x, boss_y, start_speed, speed, angle1, angle_bullet);
            INVOKE_TASK(bullet_slow_then_turn, p, boss_x, boss_y, start_speed, speed, angle2, -angle_bullet);
            
            angle1 += 360.0 / ARGS.nb_bullet_per_ring; 
            angle2 += 360.0 / ARGS.nb_bullet_per_ring;
        }
        PlaySound(sfx[SFX_TAN02]);

        angle1 += ARGS.rotation_spiral;
        angle2 -= ARGS.rotation_spiral;

        WAIT(ARGS.pause_between_rings);
    }
}

TASK(fast_rings, {Pool *pool; Entity boss; int nb_bullet_per_ring; int pause_between_rings; float speed; }) {
    Pool *p = ARGS.pool;
    WAIT(120);

    while(true) {
        float boss_x = obj_GetX(p, ARGS.boss);
        float boss_y = obj_GetY(p, ARGS.boss);

        float angle = GetRandomValue(0, 360);
        for (int i=0; i < ARGS.nb_bullet_per_ring; i++) {
            Entity bullet = Bullet_enemy_spawn_delayed(p, boss_x, boss_y, ARGS.speed, angle, BUBBLE_RED, 10);
            obj_SetRenderPriority(p, bullet, RENDER_PRIO_BULLET + 1);
            
            angle += 360.0 / ARGS.nb_bullet_per_ring;
        }
        PlaySound(sfx[SFX_TAN00]);

        WAIT(ARGS.pause_between_rings);
    }
}

DEFINE_EXTERN_TASK(moriya_nonspell_3) {
    Pool *p = ARGS.pool;
    TASK_BIND(ARGS.boss);

    INVOKE_SUBTASK(spiral_of_turn_bullets, p, ARGS.boss, 10, 5, 3);
    INVOKE_SUBTASK(fast_rings, p, ARGS.boss, 20, 80, 9);

    STALL;
}