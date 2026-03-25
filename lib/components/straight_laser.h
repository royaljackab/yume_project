#pragma once

#include "systems/timer.h"
#include "component.h"
#include "assets.h"

#include "components/common.h"
#include "components/sprite.h"

typedef struct Pool Pool;

typedef struct {
    float laserLength;
    float laserWidth;                 //taille actuelle affichée
    float laserMaxWidth;              //taille quand le laser s'élargit au max
    float intersectionWidth;          //largeur hitbox

    // float invalidLengthBase;       //Ces champs n'étaient utilisés nul part donc je les ai mis en commentaire
    // float invalidLengthTip;
    
    Timer timer;
} Straight_laser;

DEFINE_COMPONENT_MANAGER(Straight_laser, MAX_ENTITIES)
DECLARE_SETTER_GETTER(Straight_laser, float, laserLength)
DECLARE_SETTER_GETTER(Straight_laser, float, laserWidth)
DECLARE_SETTER_GETTER(Straight_laser, float, laserMaxWidth)
DECLARE_SETTER_GETTER(Straight_laser, float, intersectionWidth)

bool straight_laser_update(Straight_laser *laser);
void straight_lasers_update_all(Pool *pool);
void straight_laser_draw(Straight_laser *laser, Position * pos, Sprite * sprite);
void straight_lasers_draw_all(Straight_laserManager *laserManager, PositionManager * positionManager, SpriteManager * spriteManager);
Entity straight_laser_create(Pool *pool, int x, int y, int angle, int length, int maxWidth, int warning, int growing, int duration, SpriteID graphic);