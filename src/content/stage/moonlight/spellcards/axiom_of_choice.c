#include "spellcards/axiom_of_choice.h"
#include "assets.h"
#include "bullet.h"
#include "obj.h"
#include "physics.h"
#include "player.h"
#include "screen.h"
#include "tasks.h"
#include <raylib.h>
#include <raymath.h>

#include <math.h>

TASK(bullet_slide_logic, {Pool *p; Entity bullet; float shield_radius; bool penetrating;}) {
    Pool *p = ARGS.p;
    Entity b = ARGS.bullet;
    
    TASK_BIND(b);

    while(true) {

        if (!ARGS.penetrating) {
            Entity player = Player_get_playerID(p);
            Vector2 p_pos = obj_GetPosition(p, player);
            Vector2 b_pos = obj_GetPosition(p, b);
            
            float dist = Vector2Distance(p_pos, b_pos);

            if (dist < ARGS.shield_radius && dist > 1.0f) {

                Vector2 diff = Vector2Subtract(b_pos, p_pos);
                Vector2 repulsion = Vector2Normalize(diff);
                
                float push_force = (1.0f - dist / ARGS.shield_radius) * 0.5f;

                float angle_rad = obj_GetAngle(p, b) * DEG2RAD;
                float current_speed = obj_GetSpeed(p, b);
                
                Vector2 velocity = {
                    cosf(angle_rad) * current_speed,
                    sinf(angle_rad) * current_speed
                };

                velocity.x += repulsion.x * push_force;
                velocity.y += repulsion.y * push_force;

                float new_speed = Vector2Length(velocity);
                float new_angle = atan2f(velocity.y, velocity.x) * RAD2DEG;

                obj_SetAngle(p, b, new_angle);
                obj_SetSpeed(p, b, new_speed);
            }
        }
        YIELD;
    }
}

TASK(normal_bullet_rain_gpt, {Pool *p; int nb_bullets_per_rain; int rain_pause; float speed;}) {
    Pool *p = ARGS.p;
    const float down = 90.0f;

    while(true) {
        for (int i = 0; i < ARGS.nb_bullets_per_rain; i++) {
            float x = GetRandomValue(PANEL_LEFT, PANEL_RIGHT);
            float angle_offset = GetRandomValue(-15, 15);

            bool is_pen = (GetRandomValue(0, 5) == 0);
            int sprite = PETAL_CYAN;

            Entity b = Bullet_enemy_spawn_delayed(p, x, PANEL_UP - 10, ARGS.speed, down + angle_offset, sprite, 5);
            
            INVOKE_SUBTASK(bullet_slide_logic, p, b, 230.0f, is_pen);
        }

        WAIT(ARGS.rain_pause);
    }
}

DEFINE_EXTERN_TASK(axiom_of_choice) {
    Pool *p = ARGS.pool;

    INVOKE_SUBTASK(normal_bullet_rain_gpt, p, 15, 10, 5);
    INVOKE_SUBTASK(normal_bullet_rain_gpt, p, 15, 8, 5);


    STALL;
}