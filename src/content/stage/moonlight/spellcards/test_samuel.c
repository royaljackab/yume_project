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



TASK(spawn_random_enemy, {Pool * pool; int side;}){
    int random =  GetRandomValue(0, 1);
    int x, y;
    Entity e;
    if(ARGS.side == 1){
        x = PANEL_RIGHT;
    }
    else{
        x = PANEL_LEFT;
    }
    y = PANEL_UP * 0.8;
    if (random == 0){
        e =Enemy_spawn(ARGS.pool, x, y, 2, 1, 1, 10, 50, BULLET_M_BLUE);
    }
    else{
        e = Enemy_spawn_score_decrease(ARGS.pool, x, y, 2, 1, 10, 50, BULLET_M_BLUE);
    }

    INVOKE_SUBTASK(enemy_movement, ARGS.pool, e);
    YIELD;
}

TASK(enemy_movement, {Pool *pool; Entity enemy; }) {
    while(true) {
        float player_x = Player_GetX(ARGS.pool);
        float enemy_x = obj_GetX(ARGS.pool, ARGS.enemy);

        float angleT = 0;

        if (enemy_x <= player_x) {
            angleT = GetRandomValue(-45, 45);
        } else {
            angleT = GetRandomValue(135,215);
        }

        obj_SetAngle(ARGS.pool, ARGS.enemy, angleT);
        obj_SetSpeed(ARGS.pool, ARGS.enemy, 2);

        WAIT(60);
        obj_SetSpeed(ARGS.pool, ARGS.enemy, 0);
        WAIT(120);
    }
}



TASK(binded_ring, {Pool * pool; float spawn_x; float spawn_y; float speed; int nb_bullets; Entity Owner;}){
//** Crée un anneau de bullets qui sont "owned" par une entité */
    for(int i = 0; i<ARGS.nb_bullets; i++){
        Entity e =Bullet_enemy_spawn_delayed(ARGS.pool, ARGS.spawn_x, ARGS.spawn_y, ARGS.speed, (360/ARGS.nb_bullets)*i, BULLET_YELLOW, 5);
        Owner_bind(ARGS.pool, ARGS.Owner, e); //bind la bullet a l'owner
    }
    YIELD;
}


TASK(moving_ring_bullets,{Pool * pool; float spawn_x; float spawn_y; float radius; float angle; float param_angvel; float param_treshold; float param_burst}){
    Entity arrow = particle_spawn(ARGS.pool, ARGS.spawn_x, ARGS.spawn_y, 5, ARGS.angle, GREEN_ARROW);
    Entity centerID = Position_create(ARGS.pool, ARGS.spawn_x, ARGS.spawn_y, ARGS.angle);
    Owner_bind(ARGS.pool, centerID, arrow);

    float timer = 1;
    while (true)
    {
        obj_SetSpeed(ARGS.pool, centerID, ARGS.param_angvel/(ARGS.param_burst*timer));
        Position *position = Position_get(&ARGS.pool->position, centerID);
        if(!position){
            pool_kill_entity(ARGS.pool, arrow);
            STALL;
        }

        timer++;
        YIELD;
    }
    

}


DEFINE_EXTERN_TASK(moving_ring){
    TASK_BIND(ARGS.boss);
    INVOKE_SUBTASK(moving_ring_bullets, ARGS.pool, ARGS.boss);
    int side = 1;
    while(true){
        WAIT(200);
        for(int i = 0; i < 3; i ++){
            INVOKE_SUBTASK(spawn_random_enemy, ARGS.pool, side);
            WAIT(125);
        }
        side *= -1;
    }
}