#include "systems/timer.h"
#include "components/straight_laser.h"
#include "components/sprite.h"
#include "components/collision_rectangle.h"
#include "components/common.h"
#include "assets.h"
#include "pool.h"

#include "stdio.h"

bool straight_laser_update(Pool *p, Straight_laser *laser) {   
    /***
     * Met à jour un laser droit
     * @return true si le laser doit continuer à exister, false s'il doit être supprimé
     */

    int warning = -1;
    int growing = 0;
    int duration = 1;

    Timer timer = laser->timer;
    printf("temps %d | chrono %d\n",timer_current_time(&timer),timer.chrono);
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
    Collision_rectangle * collision = Straight_laser_get_collision(p, laser);
    collision->width = laser->laserWidth;
    collision->length = laser->laserLength;

    //mise a jour du timer
    laser->timer.chrono++;

    //le laser continue d'exister
    return true;
}

void straight_laser_create(Pool *pool, int x, int y, int angle, int length, int maxWidth, int warning, int growing, int duration, SpriteID graphic){
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

    Collision_rectangle collision = {0 , 0};
    Collision_rectangle_add(&pool->collision_rectangle, id, collision);

    printf("Laser cree\n");
    for(int i = 0; i < timer.nbTime; i++){
        printf("temps a: %d\n",timer.time[i]);
    }
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
        if(!straight_laser_update(pool, laser)) {
            printf("mise a la casse du laser %d\n",pool->straightLaser.entity_lookup[i]);
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
        pos->pos.x,
        pos->pos.y,
        laser->laserWidth,
        laser->laserLength
    };

    Vector2 origin = {laser->laserWidth/2.0, 0};

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

extern Collision_rectangle * Straight_laser_get_collision(Pool *p, Straight_laser * laser){
    /**
     * Récupère la collision d'un laser
     * Actuellement ne fonctionne pas
     */

    Collision_rectangleManager *manager = &p->collision_rectangle;
    int lookup = manager->entity_lookup[0];
    return &manager->dense[lookup];

}