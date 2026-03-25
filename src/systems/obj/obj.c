#include "obj/obj.h"
#include "common.h"
#include "pool.h"
#include "sprite.h"

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

    
}