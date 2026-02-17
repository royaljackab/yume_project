#pragma once

#include "bullet_sprites.h"
ObjID CreateShotType(ObjType type, float x, float y, float speed, float angle, BulletGraphic graphic, int delay);
ObjID CreateShot    (float x, float y, float speed, float angle, BulletGraphic graphic, int delay);
ObjID CreateLaser   (float x, float y, float angle, float length, float maxWidth, int deleteTime, BulletGraphic graphic, int delay);
ObjID CreateLooseLaser(float x, float y, float speed, float angle, float length, float width, Color color, int delay);
