#include "spellcards/brouwer_fixed_point.h"
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

TASK(spiral_multi_shot, {Pool * pool; float spawn_x; float spawn_y; float speed; float angle; int nbBursts; float param_angle_variation; float nbWaves; int direction;}){
    
    for(int j = 0; j < ARGS.nbWaves; j++){
        PlaySound(sfx[SFX_TAN00]);
        for(int i = 0; i < ARGS.nbBursts ; i++){
            Bullet_enemy_spawn_radius_delayed(ARGS.pool, ARGS.spawn_x, ARGS.spawn_y, ARGS.speed, ARGS.angle + (360/ARGS.nbBursts)*(i + ARGS.direction *j/ARGS.param_angle_variation ), 40, BULLET_M_BLUE, 10);
            // Bullet_enemy_spawn_delayed(ARGS.pool, ARGS.spawn_x, ARGS.spawn_y, ARGS.speed, ARGS.angle +(360/ARGS.nbBursts)*(i + ARGS.direction *j/ARGS.param_angle_variation ), BULLET_M_BLUE, 10);
        }
        WAIT(3);  
    }
}

TASK(clock,{Pool * pool; float spawn_x; float spawn_y; float radius; float angle; float param_angvel; float param_treshold; float param_burst}){
    PlaySound(sfx[SFX_TAN01]);
    Entity leadingID = Bullet_enemy_spawn_delayed(ARGS.pool, ARGS.spawn_x, ARGS.spawn_y, 0, ARGS.angle, BULLET_RING_YELLOW, 5);

    float timer = 1;
    Entity hour_hand = straight_laser_enemy_create(ARGS.pool, 0, 0, GetRandomValue(0, 360), 2*ARGS.radius/3, 20, INFINITE_TIME,5,150,PETAL_BLUE);
    Entity minute_hand = straight_laser_enemy_create(ARGS.pool, 0, 0, GetRandomValue(0, 360), ARGS.radius, 15, INFINITE_TIME,5,150,BULLET_RED);
    bool laserDone = false;

    Owner_bind(ARGS.pool, leadingID, hour_hand);
    Owner_bind(ARGS.pool, leadingID, minute_hand);
    
    while(true){
        obj_SetSpeed(ARGS.pool, leadingID, ARGS.param_angvel/(ARGS.param_burst*timer));
        Position *position = Position_get(&ARGS.pool->position, leadingID);
        if(!position){
            pool_kill_entity(ARGS.pool,hour_hand);
            pool_kill_entity(ARGS.pool,minute_hand);
            STALL;
        }
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
                    PlaySound(sfx[SFX_LAZER01]);
                }
                if(timer_current_time(&laser_hour->timer) == 1) {
                    laserDone = true;
                }
            }
            else{
                Position *position = Position_get(&ARGS.pool->position, leadingID);
                if(position && laserDone){
                    INVOKE_SUBTASK(basic_ring, ARGS.pool, position->pos.x, position->pos.y, 2.5, 7);
                    pool_kill_entity(ARGS.pool,leadingID);  
                }
                break;
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

TASK(clock_repeater,{Pool * pool; Entity boss;}){
    while(true){
        break;
    }
}

DEFINE_EXTERN_TASK(brouwer_fixed_point){
    TASK_BIND(ARGS.boss);
    // INVOKE_SUBTASK(clock_repeater, ARGS.pool, ARGS.boss);
    int direction = 1;
    while(true){
        // WAIT(200);

                Entity player = Player_get_playerID(ARGS.pool);
        float player_x = obj_GetX(ARGS.pool, player);
        float player_y = obj_GetY(ARGS.pool, player);
        float boss_x = obj_GetX(ARGS.pool, ARGS.boss);
        float boss_y = obj_GetY(ARGS.pool, ARGS.boss);

        float angle = atan2f(player_y - boss_y, player_x - boss_x) * RAD2DEG;

        
        for(int i = 0; i < 10; i++){
            INVOKE_SUBTASK(clock, ARGS.pool, boss_x, boss_y, 600, angle+(360/10)*i, 120, 0.5, 2.0);
        }
        WAIT(30);
        for(int i = 0; i < 10; i++){
            INVOKE_SUBTASK(clock, ARGS.pool, boss_x, boss_y, 600, angle+(360/10)*i, 120, 0.5, 1.5);
        }
        WAIT(30);
        for(int i = 0; i < 10; i++){
            INVOKE_SUBTASK(clock, ARGS.pool, boss_x, boss_y, 600, angle+(360/10)*i, 120, 0.5, 1.0);
        }

        WAIT(200);

        for(int i = 0; i < 3; i ++){
            Entity player = Player_get_playerID(ARGS.pool);
            float angle;
            float boss_x = obj_GetX(ARGS.pool, ARGS.boss);
            float boss_y = obj_GetY(ARGS.pool, ARGS.boss);
            float player_x = obj_GetX(ARGS.pool, player);
            float player_y = obj_GetY(ARGS.pool, player);
            angle = atan2f(player_y - boss_y, player_x - boss_x) * RAD2DEG;
            
            angle += GetRandomValue(-5, 5);

            INVOKE_SUBTASK(spiral_multi_shot, ARGS.pool, boss_x, boss_y, 4, angle, 16, 5, 16, direction);
            WAIT(75);
        }
        direction *= -1;
    }
}