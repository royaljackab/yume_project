#include "moonlight/nonspells/nonspell1.h"
#include "assets.h"
#include "bullet.h"
#include "obj.h"
#include "tasks.h"
#include <raylib.h>

TASK(slow_ring, {Pool *pool; Entity boss; int nb_bullets; float speed_slow; float speed_mid; float speed_fast; int periode; }) {
    int timer = 0;
    while(true) {
        float x = obj_GetX(ARGS.pool, ARGS.boss);
        float y = obj_GetY(ARGS.pool, ARGS.boss);

        float init_speed = ARGS.speed_slow + 10;
        float init_speed_fast = ARGS.speed_fast + 10;
        float init_speed_mid = ARGS.speed_mid + 10;

        float angleT = GetRandomValue(0, 360);
        for (int  i = 0; i < ARGS.nb_bullets; ++i) {
            Entity bullet = Bullet_enemy_spawn_delayed(ARGS.pool, x, y, init_speed, angleT, BULLET_YELLOW, 5);
            obj_SetAcceleration(ARGS.pool, bullet, -0.5);
            obj_SetMinSpd(ARGS.pool, bullet, ARGS.speed_slow);

            Entity bullet_mid = Bullet_enemy_spawn_delayed(ARGS.pool, x, y, init_speed_mid, angleT, BULLET_YELLOW, 5);
            obj_SetAcceleration(ARGS.pool, bullet_mid, -0.5);
            obj_SetMinSpd(ARGS.pool, bullet_mid, ARGS.speed_mid);

            Entity bullet_fast = Bullet_enemy_spawn_delayed(ARGS.pool, x, y, init_speed_fast, angleT, BULLET_YELLOW, 5);
            obj_SetAcceleration(ARGS.pool, bullet_fast, -0.5);
            obj_SetMinSpd(ARGS.pool, bullet_fast, ARGS.speed_fast);

            angleT += 360.0 / ARGS.nb_bullets;
        }
        PlaySound(sfx[SFX_TAN00]);

        Entity id_laser = loose_laser_create(ARGS.pool, x, y, 2, 4, 2, 60, GREEN);
        printf("SPEED LASER : %f | ANG SPEED : %f\n", obj_looseHead_GetAcceleration(ARGS.pool, id_laser));
        if(timer>300){
            obj_looseHead_SetSpeed(ARGS.pool, id_laser, 10);
            obj_looseHead_SetAngularSpeed(ARGS.pool, id_laser, 60);
        }
        
        timer++;
        WAIT(ARGS.periode);
    }
}

TASK(fast_spiral, {Pool *pool; Entity boss; int nb_bullets; int nb_legs; float rotation; float speed; float angle_offset; int periode; }) {
    float angleT = GetRandomValue(0, 360);
    while(true) {

        float x = obj_GetX(ARGS.pool, ARGS.boss);
        float y = obj_GetY(ARGS.pool, ARGS.boss);

        for (int j=0; j < ARGS.nb_legs; ++j) {

            float angle_curr_offset = ARGS.nb_bullets / 2 * (-ARGS.angle_offset);
            for (int i=0; i < ARGS.nb_bullets; ++i) {
                Bullet_enemy_spawn_delayed(ARGS.pool, x, y, ARGS.speed, angleT - angle_curr_offset, PETAL_LIGHT_BLUE, 5);
                angle_curr_offset += ARGS.angle_offset;
            }

            angleT += 360.0 / ARGS.nb_legs;
        }
        PlaySound(sfx[SFX_TAN02]);

        angleT += 360.0 / ARGS.nb_legs + ARGS.rotation;

        WAIT(ARGS.periode);
    }
}

DEFINE_EXTERN_TASK(moriya_nonspell_1) {
    INVOKE_SUBTASK(slow_ring, ARGS.pool, ARGS.boss, 60, 2, 4, 6, 60);
    INVOKE_SUBTASK(fast_spiral, ARGS.pool, ARGS.boss, 3, 3, 8, 4, 2.2, 5);


    STALL;
}