//TODO: Eliminer le fichier décadent
////// DEPRECATED //////

#include "bullet_sprites.h"
#include "globals.h"

Sprite bulletSprites[MAX_TEXTURES];

void Load_BulletSprites() {

    SetTexture(&bulletSprites[ANIM_TEST], ANIM_TEST_SPRITESHEET);
    SetSourceRect(&bulletSprites[ANIM_TEST], 0,0,256,16);
    SetAnimation(&bulletSprites[ANIM_TEST], 16, 2);
    SetCollisionRadius(&bulletSprites[ANIM_TEST], 5);

    SetTexture(&bulletSprites[BALL_M_BLACK], BULLET_SPRITESHEET);
    SetSourceRect(&bulletSprites[BALL_M_BLACK], 5, 54, 16, 16);
    SetCollisionRadius(&bulletSprites[BALL_M_BLACK], 5);

    SetTexture(&bulletSprites[REIMU_PINK_AMULET], REIMU_SPRITESHEET);
    SetSourceRect(&bulletSprites[REIMU_PINK_AMULET], 138, 178, 56, 14);
    SetCenter(&bulletSprites[REIMU_PINK_AMULET], 46, 6);
    SetCollisionRadius(&bulletSprites[REIMU_PINK_AMULET], 7);
    SetRotation(&bulletSprites[REIMU_PINK_AMULET], -90);

    SetTexture(&bulletSprites[RED_LASER], BULLET_SPRITESHEET);
    SetSourceRect(&bulletSprites[RED_LASER], 37,7,16,14);
}

void ObjSprite2D_SetBulletSprite(ObjID id, BulletGraphic graphic) {
    if (id==ID_INVALID) return;
    objects[id].sprite = bulletSprites[graphic];
}