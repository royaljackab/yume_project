#include "screen_effects.h"
#include <raylib.h>
#include "obj.h"

Vector2 invert_centers[4] = { {0} };
float invert_radius = 0;

DEFINE_EXTERN_TASK(player_hit_effect) {
    Pool *p =ARGS.pool;

    const int duration = 500;
    const float offset = 50;

    float px = obj_GetX(p, ARGS.player);
    float py = obj_GetY(p, ARGS.player);

    py = GetScreenHeight() - py;

    for (int i=0; i < duration; i++) {

        float ease = 1 - powf(1 - (float)i/50, 3);
        invert_radius = ease * 200;

        invert_centers[0] = (Vector2){px + offset, py + offset};
        invert_centers[1] = (Vector2){px - offset, py - offset};
        invert_centers[2] = (Vector2){px - offset, py + offset};
        invert_centers[3] = (Vector2){px + offset, py - offset};

        YIELD;
    }

    invert_radius = 0;
}