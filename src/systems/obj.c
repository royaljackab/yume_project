#include "obj.h"
#include "collision_circle.h"
#include "common.h"
#include "life.h"
#include "physics.h"
#include "pool.h"
#include "sprite.h"
#include <raylib.h>



void obj_Delete(Pool *p, Entity objId) {
    pool_kill_entity(p, objId);
}

void obj_SetVisible(Pool *p, Entity objId, bool visible) {
    Sprite *sprite = Sprite_get(&p->sprite, objId);
    if (!sprite) return;
    sprite->display = visible;
}

bool obj_IsVisible(Pool *p, Entity objId) {
    Sprite *sprite = Sprite_get(&p->sprite, objId);
    if (!sprite) return false;
    return sprite->display;
}

void obj_SetRenderPriority(Pool *p, Entity objId, int renderPriority) {
    Sprite *sprite = Sprite_get(&p->sprite, objId);
    if (!sprite) return;

    sprite->renderPriority = renderPriority;
}

int obj_GetRenderPriority(Pool *p, Entity objId) {
    Sprite *sprite = Sprite_get(&p->sprite, objId);
    if (!sprite) return -1;

    return sprite->renderPriority;
}

EntityType obj_GetType(Pool *p, Entity objId) {
    Tag *tag = Tag_get(&p->tag, objId);
    if (!tag) return ID_INVALID;

    return *tag;
}

/*----------------------------------------------------*/

void obj_SetX(Pool *p, Entity objId, float x) {
    Position *pos = Position_get(&p->position, objId);
    if (!pos) return;

    pos->pos.x = x;
}

void obj_SetY(Pool *p, Entity objId, float y) {
    Position *pos = Position_get(&p->position, objId);
    if (!pos) return;

    pos->pos.y = y;
}

void obj_SetPosition(Pool *p, Entity objId, float x, float y) {
    Position *pos = Position_get(&p->position, objId);
    if (!pos) return;

    pos->pos.x = x;
    pos->pos.y = y;
}

void obj_SetAngle(Pool *p, Entity objId, float angle) {
    Position *pos = Position_get(&p->position, objId);
    if (!pos) return;

    pos->angle = angle;
}

void obj_SetScaleX(Pool *p, Entity objId, float scalex) {
    Sprite *sprite = Sprite_get(&p->sprite, objId);
    if (!sprite) return;

    sprite->scale.x = scalex;
}

void obj_SetScaleY(Pool *p, Entity objId, float scaley) {
    Sprite *sprite = Sprite_get(&p->sprite, objId);
    if (!sprite) return;

    sprite->scale.y = scaley;
}

void obj_SetScale(Pool *p, Entity objId, float scalex, float scaley) {
    Sprite *sprite = Sprite_get(&p->sprite, objId);
    if (!sprite) return;

    sprite->scale.x = scalex;
    sprite->scale.y = scaley;
}

void obj_SetColor(Pool *p, Entity objId, float r, float g, float b) {
    Sprite *sprite = Sprite_get(&p->sprite, objId);
    if (!sprite) return;

    sprite->color.r = r;
    sprite->color.g = g;
    sprite->color.b = b;
}

void obj_SetAlpha(Pool *p, Entity objId, float alpha) {
    Sprite *sprite = Sprite_get(&p->sprite, objId);
    if (!sprite) return;

    sprite->color.a = alpha;
}


float obj_GetX(Pool *p, Entity objId) {
    Position *pos = Position_get(&p->position, objId);
    if (!pos) return 0;

    return pos->pos.x;
}

float obj_GetY(Pool *p, Entity objId) {
    Position *pos = Position_get(&p->position, objId);
    if (!pos) return 0;

    return pos->pos.y;
}

Vector2 obj_GetPosition(Pool *p, Entity objId) {
    Position *pos = Position_get(&p->position, objId);
    if (!pos) return (Vector2){0,0};

    return pos->pos;
}

float obj_GetAngle(Pool *p, Entity objId) {
    Position *pos = Position_get(&p->position, objId);
    if (!pos) return 0;

    return pos->angle;
}

float obj_GetScaleX(Pool *p, Entity objId) {
    Sprite *sprite = Sprite_get(&p->sprite, objId);
    if (!sprite) return 0;

    return sprite->scale.x;
}

float obj_GetScaleY(Pool *p, Entity objId) {
    Sprite *sprite = Sprite_get(&p->sprite, objId);
    if (!sprite) return 0;

    return sprite->scale.y;
}

Vector2 obj_GetScale(Pool *p, Entity objId) {
    Sprite *sprite = Sprite_get(&p->sprite, objId);
    if (!sprite) return (Vector2){0,0};

    return sprite->scale;
}

Color obj_GetColor(Pool *p, Entity objId) {
    Sprite *sprite = Sprite_get(&p->sprite, objId);
    if (!sprite) return WHITE;

    return sprite->color;
}

float obj_GetAlpha(Pool *p, Entity objId) {
    Sprite *sprite = Sprite_get(&p->sprite, objId);
    if (!sprite) return 0;

    return sprite->color.a;
}

/*----------------------------------------------------*/

void obj_SetAcceleration(Pool *p, Entity objId, float accel) {
    Physics *phy = Physics_get(&p->physics, objId);
    if (!phy) return;

    phy->accel = accel;
}

void obj_SetAngularSpeed(Pool *p, Entity objId, float angvel) {
    Physics *phy = Physics_get(&p->physics, objId);
    if (!phy) return;

    phy->angVel = angvel;
}

void obj_SetForceX(Pool *p, Entity objId, float forcex) {
    Physics *phy = Physics_get(&p->physics, objId);
    if (!phy) return;

    phy->force.x = forcex;
}

void obj_SetForceY(Pool *p, Entity objId, float forcey) {
    Physics *phy = Physics_get(&p->physics, objId);
    if (!phy) return;

    phy->force.x = forcey;
}

void obj_SetForce(Pool *p, Entity objId, float forcex, float forcey) {
    Physics *phy = Physics_get(&p->physics, objId);
    if (!phy) return;

    phy->force.x = forcex;
    phy->force.y = forcey;
}

void obj_SetVelocityX(Pool *p, Entity objId, float velX) {
    Physics *phy = Physics_get(&p->physics, objId);
    if (!phy) return;

    phy->velocity.x = velX;
}

void obj_SetVelocityY(Pool *p, Entity objId, float velY) {
    Physics *phy = Physics_get(&p->physics, objId);
    if (!phy) return;

    phy->velocity.y = velY;
}

void obj_SetVelocity(Pool *p, Entity objId, float velX, float velY) {
    Physics *phy = Physics_get(&p->physics, objId);
    if (!phy) return;

    phy->velocity.x = velX;
    phy->velocity.y = velY;
}

void obj_SetMaxSpd(Pool *p, Entity objId, float maxspd) {
    Physics *phy = Physics_get(&p->physics, objId);
    if (!phy) return;

    phy->maxSpd = maxspd;
}

void obj_SetMinSpd(Pool *p, Entity objId, float minspd) {
    Physics *phy = Physics_get(&p->physics, objId);
    if (!phy) return;

    phy->minSpd = minspd;
}

void obj_UnsetMaxSpd(Pool *p, Entity objId) {
    Physics *phy = Physics_get(&p->physics, objId);
    if (!phy) return;

    phy->maxSpd = NO_MAX_SPEED;
}

void obj_UnsetMinSpd(Pool *p, Entity objId) {
    Physics *phy = Physics_get(&p->physics, objId);
    if (!phy) return;

    phy->minSpd = NO_MIN_SPEED;
}

void obj_SetSpeed(Pool *p, Entity objId, float speed) {
    Physics *phy = Physics_get(&p->physics, objId);
    if (!phy) return;

    phy->speed = speed;
}

int obj_GetLife(Pool *p, Entity objId) {
    Life *life = Life_get(&p->life, objId);
    if (!life) return 0;

    return life->life;
}

int obj_GetMaxLife(Pool *p, Entity objId) {
    Life *life = Life_get(&p->life, objId);
    if (!life) return 0;

    return life->maxLife;
}

bool obj_IsDead(Pool *p, Entity objId) {
    Life *life = Life_get(&p->life, objId);
    if (!life) return true;

    return Life_is_dead(life);
}

void obj_SetLife(Pool *p, Entity objId, int lif) {
    Life *life = Life_get(&p->life, objId);
    if (!life) return;

    life->life = lif;
}

void obj_SetMaxlife(Pool *p, Entity objId, int maxLife) {
    Life *life = Life_get(&p->life, objId);
    if (!life) return;

    life->maxLife = maxLife; 
}

Tag obj_GetTag(Pool *p, Entity objId) {
    Tag *tag = Tag_get(&p->tag, objId);
    if (!tag) return ID_INVALID;

    return *tag;
}

void obj_SetTag(Pool *p, Entity objId, Tag tag) {
    Tag *existing_tag = Tag_get(&p->tag, objId);

    if (existing_tag) {
        *existing_tag = tag;
    } else {
        Tag_add(&p->tag, objId, tag);
    }    
}

void obj_SetHitboxRadius(Pool *p, Entity objID, float radius) {
    Collision_circle *hitbox = Collision_circle_get(&p->collision_circle, objID);
    if (!hitbox) return;

    hitbox->radius = radius;
}