#ifndef BULLET_SPRITES_H
#define BULLET_SPRITES_H

#include "obj.h"

typedef enum {
    BALL_M_BLACK,
    ANIM_TEST,
    REIMU_PINK_AMULET,
    RED_LASER
} BulletGraphic;

void Load_BulletSprites();
void ObjSprite2D_SetBulletSprite(ObjID id, BulletGraphic graphic);

#endif