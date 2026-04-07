#include "spellcards/gram_schmidt.h"
#include "assets.h"
#include "bullet.h"
#include "ecs.h"
#include "systems/obj.h"
#include "screen.h"
#include "tasks.h"
#include "common.h"
#include <raylib.h>

#include "stdio.h"

#define INFINITE_TIME 99999

TASK(basic_ring, {Pool * pool; float spawn_x; float spawn_y; float speed; int nb_bullets;}){
    for(int i = 0; i<ARGS.nb_bullets; i++){
        Bullet_enemy_spawn_delayed(ARGS.pool, ARGS.spawn_x, ARGS.spawn_y, ARGS.speed, (360/ARGS.nb_bullets)*i, BULLET_YELLOW, 5);
    }
    YIELD;
}

TASK(triple_shot, {Pool * pool; float spawn_x; float spawn_y; float speed; float angle;}){
    for(int i = 0; i<5; i++){
        Bullet_enemy_spawn_delayed(ARGS.pool, ARGS.spawn_x, ARGS.spawn_y, ARGS.speed, ARGS.angle+20, BULLET_M_BLUE, 5);
        Bullet_enemy_spawn_delayed(ARGS.pool, ARGS.spawn_x, ARGS.spawn_y, ARGS.speed*1.2, ARGS.angle, BULLET_M_BLUE, 5);
        Bullet_enemy_spawn_delayed(ARGS.pool, ARGS.spawn_x, ARGS.spawn_y, ARGS.speed, ARGS.angle-20, BULLET_M_BLUE, 5);
        WAIT(5);
    }
}

TASK(clock,{Pool * pool; float spawn_x; float spawn_y; float radius; float angle; float param_angvel; float param_treshold; float param_burst}){
    Entity leadingID = Bullet_enemy_spawn_delayed(ARGS.pool, ARGS.spawn_x, ARGS.spawn_y, 0, ARGS.angle, BULLET_RING_YELLOW, 5);

    float timer = 1;
    Entity hour_hand = straight_laser_enemy_create(ARGS.pool, 0, 0, GetRandomValue(0, 360), 2*ARGS.radius/3, 20, INFINITE_TIME,5,150,LASER_CYAN);
    Entity minute_hand = straight_laser_enemy_create(ARGS.pool, 0, 0, GetRandomValue(0, 360), ARGS.radius, 15, INFINITE_TIME,5,150,LASER_RED);

    Owner_bind(ARGS.pool, leadingID, hour_hand);
    Owner_bind(ARGS.pool, leadingID, minute_hand);
    
    while(true){
        obj_SetSpeed(ARGS.pool, leadingID, ARGS.param_angvel/(ARGS.param_burst*timer));

        // printf("angvel hour : %f\n",obj_GetAngularSpeed(ARGS.pool,hour_hand));
        // printf("moins que 0,1 : %d\n",(obj_GetAngularSpeed(ARGS.pool,hour_hand) < 0.1 ));
        // printf("id hour : %d\n",hour_hand);

        //les deux aiguilles passent la barre des param_time en même temps, j'ai calculé
        if( (obj_GetAngularSpeed(ARGS.pool,hour_hand) < ARGS.param_treshold ) && timer != 1){
            Straight_laser *laser_hour = Straight_laser_get(&ARGS.pool->straightLaser, hour_hand);
            Straight_laser *laser_minute = Straight_laser_get(&ARGS.pool->straightLaser, minute_hand);
            if(laser_hour && laser_minute){
                obj_SetAngularSpeed(ARGS.pool, minute_hand, 0);
                obj_SetAngularSpeed(ARGS.pool, hour_hand, 0);

                //on manipule la notion de timer des lasers pour activer leur agrandissement au moment voulu (utilisation détournée)
                if(laser_hour->timer.chrono < INFINITE_TIME) {
                    laser_hour->timer.chrono = INFINITE_TIME;
                    laser_minute->timer.chrono = INFINITE_TIME;
                }
            }
            else{
                Position *position = Position_get(&ARGS.pool->position, leadingID);
                if(position){
                    INVOKE_SUBTASK(basic_ring, ARGS.pool, position->pos.x, position->pos.y, 2.5, 20);
                    pool_kill_entity(ARGS.pool,leadingID);
                    STALL;
                }
                
            }


        }
        else{
            obj_SetAngularSpeed(ARGS.pool, hour_hand, ARGS.param_angvel/timer);
            obj_SetAngularSpeed(ARGS.pool, minute_hand,(float)(1/ARGS.param_treshold)*ARGS.param_angvel*ARGS.param_angvel/(timer * timer));
        }

        timer++;
        YIELD;
    }
}

DEFINE_EXTERN_TASK(orthonormalisation){
    TASK_BIND(ARGS.boss);
    
    while(true){
        Entity player = Player_get_playerID(ARGS.pool);
        float angle;
        float boss_x = obj_GetX(ARGS.pool, ARGS.boss);
        float boss_y = obj_GetY(ARGS.pool, ARGS.boss);
        float player_x = obj_GetX(ARGS.pool, player);
        float player_y = obj_GetY(ARGS.pool, player);

        angle = atan2f(player_y - boss_y, player_x - boss_x) * RAD2DEG;
        angle += GetRandomValue(-5, 5);

        INVOKE_SUBTASK(triple_shot, ARGS.pool, boss_x, boss_y, 4, angle);
        WAIT(75);

        angle = atan2f(player_y - boss_y, player_x - boss_x) * RAD2DEG;
        angle += GetRandomValue(-30, 30);

        INVOKE_SUBTASK(clock, ARGS.pool, obj_GetX(ARGS.pool,ARGS.boss), obj_GetY(ARGS.pool,ARGS.boss), 600, angle, 120, 0.5, 1.5 + GetRandomValue(-0.5, 0));
        WAIT(25);
    }
}