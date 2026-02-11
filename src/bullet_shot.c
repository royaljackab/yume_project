//TODO: Eliminer le fichier décadent
////// DEPRECATED //////

#include "bullet_shot.h"
#include "globals.h"

ObjID CreateShotType(ObjType type, float x, float y, float speed, float angle, BulletGraphic graphic, int delay) {
    int id = Obj_Create(type);

    ObjMove_SetPosition(id, x, y);
    ObjMove_SetSpeed(id, speed);
    ObjMove_SetAngle(id, angle);

    ObjSprite2D_SetBulletSprite(id, graphic);

    Obj_SetDelay(id, delay);

    return id;
}


ObjID CreateShot(float x, float y, float speed, float angle, BulletGraphic graphic, int delay) {
    return CreateShotType(OBJ_ENEMY_SHOT, x, y, speed, angle, graphic, delay);
}

ObjID CreateLaser(float x, float y, float angle, float length, float maxWidth, int deleteTime, BulletGraphic graphic, int delay) {
    int id = Obj_Create(OBJ_ENEMY_LASER);

    ObjMove_SetPosition(id, x, y);
    ObjMove_SetAngle(id, angle);
    
    ObjLaser_SetLength(id, length);
    ObjLaser_SetMaxWidth(id, maxWidth);
    ObjLaser_SetTimers(id, delay, 10, deleteTime);

    ObjSprite2D_SetBulletSprite(id, graphic);

    objects[id].laserWidth=0;
    objects[id].laserState=0;

    return id;
}

ObjID CreateLooseLaser(float x, float y, float speed, float angle, float length, float width, Color color, int delay) {
    int id = Obj_Create(OBJ_LOOSE_LASER);
    if (id == ID_INVALID) return ID_INVALID;

    ObjMove_SetPosition(id, x, y);
    ObjMove_SetSpeed(id, speed);
    ObjMove_SetAngle(id, angle);
    
    objects[id].looseTargetLength = length;
    objects[id].looseWidth = width;
    
    // Initialise le premier noeud à la position de départ pour éviter un glitch à la frame 1
    objects[id].looseNodes[0] = (Vector2){x, y};
    objects[id].looseNodeCount = 1;

    ObjSprite2D_SetColor(id, color);
    Obj_SetDelay(id, delay);

    return id;
}