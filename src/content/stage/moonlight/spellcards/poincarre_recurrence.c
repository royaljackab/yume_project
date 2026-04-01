#include "spellcards/poincarre_recurrence.h"
#include "assets.h"
#include "bullet.h"
#include "ecs.h"
#include "obj.h"
#include "screen.h"
#include "tasks.h"
#include <raylib.h>

#define NB_BULLETS_RING 30

TASK(bouncing_ring, {Pool *pool; float cx; float cy; float vx; float vy; float radius; float ring_angle; bool initialized; Entity bullets[NB_BULLETS_RING]; }) {

    if (!ARGS.initialized) {
        for (int i=0; i < NB_BULLETS_RING; ++i) {
            ARGS.bullets[i] = Bullet_enemy_spawn_delayed(ARGS.pool, ARGS.cx, ARGS.cy, 0, 0, BULLET_BLUE, 5);
        }
        ARGS.initialized = true;
    }

    while (true) {
        ARGS.cx += ARGS.vx;
        ARGS.cy += ARGS.vy;
        ARGS.ring_angle += 1.5f;

        float margin = ARGS.radius + 10;

        if (ARGS.cx - margin <= PANEL_LEFT) {
            ARGS.cx = PANEL_LEFT + margin;
            ARGS.vx = -ARGS.vx;
            PlaySound(sfx[SFX_TAN02]);
        } else if (ARGS.cx + margin >= PANEL_RIGHT) {
            ARGS.cx = PANEL_RIGHT - margin;
            ARGS.vx = -ARGS.vx;
            PlaySound(sfx[SFX_TAN02]);
        }

        if (ARGS.cy - margin <= PANEL_UP) {
            ARGS.cy = PANEL_UP + margin;
            ARGS.vy = -ARGS.vy;
            PlaySound(sfx[SFX_TAN02]);
        } else if (ARGS.cy + margin >= PANEL_DOWN) {
            ARGS.cy = PANEL_DOWN - margin;
            ARGS.vy = -ARGS.vy;
            PlaySound(sfx[SFX_TAN02]);
        }

        bool all_dead = true;
        for (int i=0; i < NB_BULLETS_RING; ++i) {
            Entity b = ARGS.bullets[i];

            if (b != NULL_INDEX) {
                all_dead = false;

                float angle_deg = ARGS.ring_angle + (float)i * (360.0 / NB_BULLETS_RING);
                float angle_rad = angle_deg * DEG2RAD;

                float bx = ARGS.cx + cosf(angle_rad) * ARGS.radius;
                float by = ARGS.cy + sinf(angle_rad) * ARGS.radius;

                obj_SetPosition(ARGS.pool, b, bx, by);
                obj_SetAngle(ARGS.pool, b, angle_deg);
            }
        }

        if(all_dead) break;

        YIELD;
    }
}

TASK(slow_flower, {Pool *pool; Entity boss; float speed; float rotation; int duration; int nb_legs; }) {
    TASK_BIND(ARGS.boss);

    float angle = GetRandomValue(0, 360);
    while (true) {
        float boss_x = obj_GetX(ARGS.pool, ARGS.boss);
        float boss_y = obj_GetY(ARGS.pool, ARGS.boss);

        for (int i=0; i<ARGS.nb_legs; ++i) {
            Bullet_enemy_spawn_delayed(ARGS.pool, boss_x, boss_y, ARGS.speed, angle, MISSILE_RED, 10);
            Bullet_enemy_spawn_delayed(ARGS.pool, boss_x, boss_y, ARGS.speed, -angle, MISSILE_RED, 10);

            angle += 360.0 / ARGS.nb_legs;
        }
        angle += 360.0 / ARGS.nb_legs;


        angle += ARGS.rotation;
        WAIT(ARGS.duration);
    }
}

DEFINE_EXTERN_TASK(poincarre_recurrence) {
    TASK_BIND(ARGS.boss);

    for (int i=0; i < ARGS.nb_rings; ++i) {
        float boss_x = obj_GetX(ARGS.pool, ARGS.boss);
        float boss_y = obj_GetY(ARGS.pool, ARGS.boss);

        float angle_deg = (float)i * (360.0 / ARGS.nb_rings);
        float angle_rad = angle_deg * DEG2RAD;

        float vx = cosf(angle_rad) * ARGS.speed;
        float vy = sinf(angle_rad) * ARGS.speed;

        INVOKE_SUBTASK(bouncing_ring, ARGS.pool, boss_x, boss_y, vx, vy, ARGS.ring_radius, 0, false);
        PlaySound(sfx[SFX_TAN00]);

        WAIT(5);
    }
    WAIT(30);
    INVOKE_SUBTASK(slow_flower, ARGS.pool, ARGS.boss, 4, 17, 10, 4);

    STALL;
}