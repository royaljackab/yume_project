#include "straight_laser.h"
#include "common.h"
#include "collision_rectangle.h"

bool updateStraightLaser(Straight_laser *laser, Timer *timer) {   
    /***
     * Met à jour un laser droit en fonction de son timer
     * @return true si le laser doit continuer à exister, false s'il doit être supprimé
     * 
     * 
     * 
     * 
     */         
    int warning = Straight_laser_get_warningTimer(laser);
    int growing = Straight_laser_get_growingTimer(laser);
    int duration = Straight_laser_get_laserDuration(laser);


    if(*timer < warning) {
        laser->laserWidth = 2;
        laser->laserState = 0;
    }
    else if(*timer < growing + warning) {
        laser->laserWidth += laser->laserMaxWidth / laser->growingTimer;
        laser->laserState = 1;
    }
    else if(*timer < growing + warning + duration) {
        laser->laserWidth = laser->laserMaxWidth;
        laser->laserState = 1;
    }
    else {
        laser->laserWidth -= laser->laserMaxWidth / laser->growingTimer;
        if(laser->laserWidth <= 0) {
            return false;
        }
    }
    return true;
}
void updateAllStraightLasers(Straight_laserManager *laserManager) {
    Straight_laser *laser;
    for (int i=0; i < laserManager->count; i++) {
        laser = &laserManager->dense[i];
        //TODO: détruire
    }
}