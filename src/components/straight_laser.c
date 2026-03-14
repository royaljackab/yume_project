#include "systems/timer.h"
#include "components/straight_laser.h"
#include "components/sprite.h"
#include "components/collision_rectangle.h"
#include "components/common.h"
#include "assets.h"
#include "pool.h"


bool updateStraightLaser(Straight_laser *laser) {   
    /***
     * Met à jour un laser droit en fonction de son timer
     * @return true si le laser doit continuer à exister, false s'il doit être supprimé
     */

    int warning = 0;
    int growing = 1;
    int duration = 2;

    Timer timer = laser->timer;
    if(timer_current_time(&timer) == warning) {
        laser->laserWidth = 2;
    }
    else if(timer_current_time(&timer) == growing) {
        laser->laserWidth += laser->laserMaxWidth / timer.chrono;
    }
    else if(timer_current_time(&timer) == duration) {
        laser->laserWidth = laser->laserMaxWidth;
    }
    else {
        laser->laserWidth -= laser->laserMaxWidth / timer.chrono;
        //le laser doit être détruit
        if(laser->laserWidth <= 0) {
            return false;
        }
    }

    //mise a jour du timer
    laser->timer.chrono++;

    //le laser continue d'exister
    return true;
}

void updateAllStraightLasers(Pool *pool) {
    /***
     * Met à jour tout les laser droits et les ajoute à la kill queue s'ils sont finis
     * 
     */ 
    Straight_laser *laser;
    for (int i=0; i < pool->straightLaser.count; i++) {
        laser = &pool->straightLaser.dense[i];
        if(!updateStraightLaser(laser)) {
            pool_kill_entity(pool, pool->straightLaser.entity_lookup[i]);
        }
    }
}

void draw_straight_laser(Straight_laser *laser, Position * pos, Sprite * sprite){
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

void draw_all_straight_lasers(Straight_laserManager *laserManager, PositionManager * positionManager, SpriteManager * spriteManager) {
    /**
     * Affiche tous les lasers droits actifs
     */
    Straight_laser *laser;
    Position pos;
    Sprite sprite;
    for (int i=0; i < laserManager->count; i++) {
        laser = &laserManager->dense[i];
        pos = positionManager->dense[laserManager->entity_lookup[i]];
        sprite = spriteManager->dense[laserManager->entity_lookup[i]];
        draw_straight_laser(laser, &pos, &sprite);
    }
}