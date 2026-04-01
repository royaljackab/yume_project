#include "moonlight/nonspells/nonspell1.h"
#include "assets.h"
#include "bullet.h"
#include "obj.h"
#include "tasks.h"
#include <raylib.h>

TASK(slow_ring, {Pool *pool; Entity boss; int nb_bullets; float speed_slow; float speed_fast; int periode; }) {
    while(true) {
        float x = obj_GetX(ARGS.pool, ARGS.boss);
        float y = obj_GetY(ARGS.pool, ARGS.boss);

        float init_speed = ARGS.speed_slow + 10;
        float init_speed_fast = ARGS.speed_fast + 10;
        float angleT = GetRandomValue(0, 360);
        for (int  i = 0; i < ARGS.nb_bullets; ++i) {
            Entity bullet = Bullet_enemy_spawn(ARGS.pool, x, y, init_speed, angleT, BULLET_BLACK);
            obj_SetAcceleration(ARGS.pool, bullet, -0.5);
            obj_SetMinSpd(ARGS.pool, bullet, ARGS.speed_slow);

            Entity bullet_fast = Bullet_enemy_spawn(ARGS.pool, x, y, init_speed_fast, angleT, BULLET_BLACK);
            obj_SetAcceleration(ARGS.pool, bullet_fast, -0.5);
            obj_SetMinSpd(ARGS.pool, bullet_fast, ARGS.speed_fast);

            angleT += 360.0 / ARGS.nb_bullets;
        }

        WAIT(ARGS.periode);
    }
}

TASK(fast_spiral, {Pool *pool; Entity boss; int nb_bullets; int nb_legs; float rotation; float speed; float spacing; int periode; }) {
    float angleT = GetRandomValue(0, 360);
    while(true) {

        float x = obj_GetX(ARGS.pool, ARGS.boss);
        float y = obj_GetY(ARGS.pool, ARGS.boss);

        for (int j=0; j < ARGS.nb_legs; ++j) {
            float angle_perp_rad = (angleT + 90.0) * DEG2RAD;
            float cos_p = cosf(angle_perp_rad);
            float sin_p = sinf(angle_perp_rad);
            
            float total_width = (ARGS.nb_bullets - 1) * ARGS.spacing;
            float offset = -total_width / 2.0;

            for (int i=0; i < ARGS.nb_bullets; ++i) {
                float spawn_x = x + (cos_p * offset);
                float spawn_y = y + (sin_p * offset);

                Bullet_enemy_spawn(ARGS.pool, spawn_x, spawn_y, ARGS.speed, angleT, PETAL_LIGHT_BLUE);

                offset += ARGS.spacing;
            }

            angleT += 360.0 / ARGS.nb_legs;
        }

        angleT += 360.0 / ARGS.nb_legs + ARGS.rotation;

        WAIT(ARGS.periode);
    }
}

DEFINE_EXTERN_TASK(moriya_nonspell_1) {
    INVOKE_SUBTASK(slow_ring, ARGS.pool, ARGS.boss, 15, 2, 4, 60);
    INVOKE_SUBTASK(fast_spiral, ARGS.pool, ARGS.boss, 3, 3, 5, 5, 10, 2);

    STALL;
}