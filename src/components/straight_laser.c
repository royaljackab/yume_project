#include "systems/timer.h"
#include "components/straight_laser.h"
#include "components/sprite.h"
#include "components/collision_rectangle.h"
#include "components/common.h"
#include "assets.h"
#include "pool.h"

#include "stdio.h"

bool straight_laser_update(Pool *p, Entity laserID) {   
    /***
     * Met à jour un laser droit
     * @return true si le laser doit continuer à exister, false s'il doit être supprimé
     */

    Straight_laser *laser = Straight_laser_get(&p->straightLaser, laserID);
    int warning = -1;
    int growing = 0;
    int duration = 1;

    Timer timer = laser->timer;
    // printf("temps %d | chrono %d\n",timer_current_time(&timer),timer.chrono);
    if(timer_current_time(&timer) == warning) {
        laser->laserWidth = 2;
    }
    else if(timer_current_time(&timer) == growing) {
        laser->laserWidth += laser->laserMaxWidth / (timer.time[1]-timer.time[0]);
    }
    else if(timer_current_time(&timer) == duration) {
        laser->laserWidth = laser->laserMaxWidth;
    }
    else {
        laser->laserWidth -= laser->laserMaxWidth / (timer.time[1]-timer.time[0]);
        //le laser doit être détruit
        if(laser->laserWidth <= 0) {
            return false;
        }
    }
    //mise a jour de la hitbox 
    Collision_rectangle * collision = Collision_rectangle_get(&p->collision_rectangle, laserID);
    collision->width = laser->laserWidth;
    collision->length = laser->laserLength;

    //mise a jour du timer
    laser->timer.chrono++;

    //le laser continue d'exister
    return true;
}

Entity straight_laser_create(Pool *pool, int x, int y, int angle, int length, int maxWidth, int warning, int growing, int duration, SpriteID graphic){
    Entity id = pool_create_entity(pool);
    Vector2 vect = {x,y};
    Position pos = {vect, angle};

    Timer timer;
    timer.chrono = 0;
    timer.nbTime = 0;
    timer_add_time(&timer, warning);
    timer_add_time(&timer, warning+growing);
    timer_add_time(&timer, warning+growing+duration);

    Straight_laser laser = {
        .laserLength = length,
        .laserMaxWidth = maxWidth,              //taille quand le laser s'élargit au max
        // .intersectionWidth =,          // largeur hitbox
        .timer = timer
    };
    Straight_laser_add(&pool->straightLaser, id, laser);
    Position_add(&pool->position, id, pos);
    Sprite_add(&pool->sprite,id,sprites[graphic]);
    Sprite_set_display(Sprite_get(&pool->sprite,id),0);
    Physics phy = Physics_create_speed(0);
    Physics_add(&pool->physics,id,phy);

    Collision_rectangle collision = {0 , 0};
    Collision_rectangle_add(&pool->collision_rectangle, id, collision);

    //rintf("Laser cree\n");
    for(int i = 0; i < timer.nbTime; i++){
    }
    return id;
}

void straight_lasers_update_all(Pool *pool) {
    /***
     * Met à jour tout les laser droits et les ajoute à la kill queue s'ils sont finis
     * 
     */ 
    Straight_laser *laser;
    for (int i=0; i < pool->straightLaser.count; i++) {
        laser = &pool->straightLaser.dense[i];
        // printf("mise a jour du laser %d\n",pool->straightLaser.entity_lookup[i]);
        if(!straight_laser_update(laser)) {
            // printf("mise a la casse du laser %d\n",pool->straightLaser.entity_lookup[i]);
            pool_kill_entity(pool, pool->straightLaser.entity_lookup[i]);
        }
    }
}

void straight_laser_draw(Straight_laser *laser, Position * pos, Sprite * sprite){
    /**
     * Affiche un laser droit selon sa largeur et sa longueur, à sa position et avec la couleur de son sprite
     * 
     */
    int textureID = sprite->textureID;

    Rectangle source = sprite->srcRect;

    Rectangle dest = {
        pos->pos.y,
        pos->pos.x,
        laser->laserLength,
        laser->laserWidth
    };

    Vector2 origin = {0, laser->laserWidth/2.0};

    DrawTexturePro(textures[textureID], source, dest, origin, pos->angle, sprite->color);
}

void straight_lasers_draw_all(Straight_laserManager *laserManager, PositionManager * positionManager, SpriteManager * spriteManager) {
    /**
     * Affiche tous les lasers droits actifs
     */
    Straight_laser *laser;
    Position *pos;
    Sprite *sprite;
    for (int i=0; i < laserManager->count; i++) {
        laser = &laserManager->dense[i];
        pos = Position_get(positionManager,laserManager->entity_lookup[i]);
        sprite = Sprite_get(spriteManager,laserManager->entity_lookup[i]);
        straight_laser_draw(laser, pos, sprite);
    }
}

