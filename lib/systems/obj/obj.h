#pragma once

#include "common.h"
#include "ecs.h"
#include <stdbool.h>

void obj_Delete(Pool *p, Entity objId);
void obj_SetVisible(Pool *p, Entity objId, bool visible);
bool obj_IsVisible(Pool *p, Entity objId);
void obj_SetRenderPriority(Pool *p, Entity objId, int renderPriority);
int obj_GetRenderPriority(Pool *p, Entity objId);
EntityType obj_GetType(Pool *p, Entity objId);

/*----------------------------------------------------*/

void obj_SetX(Pool *p, Entity objId, float x);
void obj_SetY(Pool *p, Entity objId, float y);
void obj_SetPosition(Pool *p, Entity objId, float x, float y);
void obj_SetAngle(Pool *p, Entity objId, float angle);
void obj_SetScaleX(Pool *p, Entity objId, float scalex);
void obj_SetScaleY(Pool *p, Entity objId, float scaley);
void obj_SetScale(Pool *p, Entity objId, float scalex, float scaley);
void obj_SetColor(Pool *p, Entity objId, float r, float g, float b);
void obj_SetAlpha(Pool *p, Entity objId, float alpha);

float obj_GetX(Pool *p, Entity objId);
float obj_GetY(Pool *p, Entity objId);
Vector2 obj_GetPosition(Pool *p, Entity objId);
float obj_GetAngle(Pool *p, Entity objId);
float obj_GetScaleX(Pool *p, Entity objId);
float obj_GetScaleY(Pool *p, Entity objId);
Vector2 obj_GetScale(Pool *p, Entity objId);
Color obj_GetColor(Pool *p, Entity objId);
float obj_GetAlpha(Pool *p, Entity objId);

/*----------------------------------------------------*/

void obj_SetAcceleration(Pool *p, Entity objId, float accel);
void obj_SetSpeed(Pool *p, Entity objId, float speed);
void obj_SetAngularSpeed(Pool *p, Entity objId, float angvel);
void obj_SetForceX(Pool *p, Entity objId, float forcex);
void obj_SetForceY(Pool *p, Entity objId, float forcey);
void obj_SetForce(Pool *p, Entity objId, float forcex, float forcey);
void obj_SetVelocityX(Pool *p, Entity objId, float velX);
void obj_SetVelocityY(Pool *p, Entity objId, float velY);
void obj_SetVelocity(Pool *p, Entity objId, float velX, float velY);
void obj_SetMaxSpd(Pool *p, Entity objId, float maxspd);
void obj_SetMinSpd(Pool *p, Entity objId, float minspd);
void obj_UnsetMaxSpd(Pool *p, Entity objId);
void obj_UnsetMinSpd(Pool *p, Entity objId);

/*----------------------------------------------------*/

int obj_GetLife(Pool *p, Entity objId);
int obj_GetMaxLife(Pool *p, Entity objId);
bool obj_IsDead(Pool *p, Entity objId);

void obj_SetLife(Pool *p, Entity objId, int life);
void obj_SetMaxlife(Pool *p, Entity objId, int maxLife);
