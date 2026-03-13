#include "systems/timer.h"
#include "straight_laser.h"
#include "common.h"
#include "pool.h"
#include "collision_rectangle.h"

bool updateStraightLaser(Straight_laser *laser) {   
    /***
     * Met à jour un laser droit en fonction de son timer
     * @return true si le laser doit continuer à exister, false s'il doit être supprimé
     * 
     * 
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
    ///TODO: Je ne sais pas comment gêrer les loops
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