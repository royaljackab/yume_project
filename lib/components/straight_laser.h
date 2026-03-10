#pragma once
#include "component.h"
typedef struct Pool Pool;
#include "common.h"

typedef struct {
    float laserLength;
    float laserWidth;                 //taille actuelle affichée
    float laserMaxWidth;              //taille quand le laser s'élargit au max
    float intersectionWidth;          // largeur hitbox
    float invalidLengthBase;          
    float invalidLengthTip;
    int laserState;                   // 0:warning 1:actif 2:inactif //peut etre inutile...
    int warningTimer;
    int growingTimer;
    int laserDuration;
} Straight_laser;

DEFINE_COMPONENT_MANAGER(Straight_laser, MAX_ENTITIES)
DECLARE_SETTER_GETTER(Straight_laser, float, laserLength)
DECLARE_SETTER_GETTER(Straight_laser, float, laserWidth)
DECLARE_SETTER_GETTER(Straight_laser, float, laserMaxWidth)
DECLARE_SETTER_GETTER(Straight_laser, float, intersectionWidth)
DECLARE_SETTER_GETTER(Straight_laser, float, invalidLengthBase)
DECLARE_SETTER_GETTER(Straight_laser, float, invalidLengthTip)
DECLARE_SETTER_GETTER(Straight_laser, int, laserState)
DECLARE_SETTER_GETTER(Straight_laser, int, warningTimer)
DECLARE_SETTER_GETTER(Straight_laser, int, growingTimer)
DECLARE_SETTER_GETTER(Straight_laser, int, laserDuration)

bool updateStraightLaser(Straight_laser *laser, Timer *timer);
void updateAllStraightLasers(Pool *ctx);