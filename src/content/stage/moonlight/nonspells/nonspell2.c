#include "nonspells/nonspell2.h"
#include "assets.h"
#include "bullet.h"
#include "cotask.h"
#include "tasks.h"
#include "obj.h"
#include <raylib.h>

TASK(fast_spiral, {Pool *pool; Entity boss; float offset_x; float offset_y; float angle; int nb_bullets; int nb_legs; float rotation; float speed; float angle_offset; int periode; }) {
    float angleT = ARGS.angle;
    while(true) {

        float x = obj_GetX(ARGS.pool, ARGS.boss) + ARGS.offset_x;
        float y = obj_GetY(ARGS.pool, ARGS.boss) + ARGS.offset_y;

        for (int j=0; j < ARGS.nb_legs; ++j) {

            float angle_curr_offset = ARGS.nb_bullets / 2 * (-ARGS.angle_offset);
            for (int i=0; i < ARGS.nb_bullets; ++i) {
                Entity bullet = Bullet_enemy_spawn_delayed(ARGS.pool, x, y, ARGS.speed, angleT - angle_curr_offset, PETAL_LIGHT_BLUE, 10);
                obj_SetRenderPriority(ARGS.pool, bullet, RENDER_PRIO_BULLET + 10);
                angle_curr_offset += ARGS.angle_offset;
            }

            angleT += 360.0 / ARGS.nb_legs;
        }
        PlaySound(sfx[SFX_TAN01]);

        angleT += 360.0 / ARGS.nb_legs + ARGS.rotation;

        WAIT(ARGS.periode);
    }
}

TASK(fat_rings, {Pool *pool; Entity boss; int nb_bullets; float speed; int periode;}) {
    while (true) {
        float x = obj_GetX(ARGS.pool, ARGS.boss);
        float y = obj_GetY(ARGS.pool, ARGS.boss);

        float angle = GetRandomValue(0, 30);
        for (int i=0; i < ARGS.nb_bullets; ++i) {
            Bullet_enemy_spawn_delayed(ARGS.pool, x, y, ARGS.speed, angle, BALL_L_BLACK, 5);
            angle += 360.0 / ARGS.nb_bullets;
        }
        PlaySound(sfx[SFX_TAN00]);

        WAIT(ARGS.periode);
    }
}

TASK(fat_rings_attack, {Pool *pool; Entity boss; int periode;}) {
    WAIT(60);
    while(true) {
        CoTask *rings = INVOKE_SUBTASK(fat_rings, ARGS.pool, ARGS.boss, 30, 3, 10);
        BoxedTask rings_box = cotask_box(rings);

        WAIT(ARGS.periode);

        CANCEL_TASK(rings_box);

        WAIT(ARGS.periode);
    }
}

DEFINE_EXTERN_TASK(moriya_nonspell_2) {

    INVOKE_SUBTASK(fat_rings_attack, ARGS.pool, ARGS.boss, 120);

    float angle = GetRandomValue(0, 359);
    INVOKE_SUBTASK(fast_spiral, ARGS.pool, ARGS.boss,30, -25, angle, 3, 4, 7, 9, 0.8, 7);
    INVOKE_SUBTASK(fast_spiral, ARGS.pool, ARGS.boss, -30, -25,angle + 90,3, 4, -7, 9, 0.8, 7);

    STALL;
}