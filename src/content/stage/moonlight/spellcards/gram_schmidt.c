#include "spellcards/poincarre_recurrence.h"
#include "assets.h"
#include "bullet.h"
#include "ecs.h"
#include "systems/obj.h"
#include "screen.h"
#include "tasks.h"
#include <raylib.h>

#define PARAM_ANGVEL 1

TASK(clock,{Pool * pool; float spawn_x; float spawn_y; float vx; float vy; float radius; float speed; float angle}){
    Entity leadingID = Bullet_enemy_spawn_delayed(ARGS.pool, ARGS.spawn_x, ARGS.spawn_y, ARGS.speed, ARGS.angle, BULLET_RING_YELLOW, 5);
    Position *position = Position_get(&ARGS.pool->position, leadingID);
    float x = position->pos.x;
    float y = position->pos.y;
    int timer = 1;
    Entity hour_hand = straight_laser_enemy_create(ARGS.pool, x, y, 0, 2*ARGS.radius/3, 10,9999,0,0,LASER_CYAN);
    Entity minute_hand = straight_laser_enemy_create(ARGS.pool, x, y, 0, ARGS.radius, 5,9999,0,0,LASER_CYAN);
    
    //les deux aiguilles passent la barre des 0,1*PARAM_ANGVEL en même temps, j'ai calculé
    if(obj_GetAngularSpeed(ARGS.pool,hour_hand) < 0,1*PARAM_ANGVEL){
        obj_SetAngularSpeed(ARGS.pool, hour_hand, 0);
        obj_SetAngularSpeed(ARGS.pool, minute_hand, 0);

        Straight_laser *laser_hour = Straight_laser_get(ARGS.pool, hour_hand);
        Straight_laser *laser_minute = Straight_laser_get(ARGS.pool, minute_hand);
        laser_hour->timer.time[0] = 0;
        laser_minute->timer.time[0] = 0;
    }
    else{
        obj_SetAngularSpeed(ARGS.pool, hour_hand, 1/timer);
        obj_SetAngularSpeed(ARGS.pool, minute_hand, 10/(timer * timer));
    }
}

DEFINE_EXTERN_TASK(orthonormalisation){
    TASK_BIND(ARGS.boss);
    STALL;
}