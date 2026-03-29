/**
 * @file enemy.c
 * @brief Implémentation du composant Enemy
 *
 * @author @amyelalem
 */

#include "components/enemy.h"
#include "components/common.h"
#include "components/life.h"
#include "components/collision_circle.h"
#include "components/physics.h"
#include "components/sprite.h"
#include "content/assets.h"
#include "ecs/pool.h"

Entity Enemy_spawn(Pool *p, float x, float y, float speed, float angle,
                   int life, float hitboxRadius,
                   EnemyType type, SpriteID graphic) {

    Entity e = pool_create_entity(p);

    Position pos = {{x, y}, angle};
    Physics phy = Physics_create_speed(speed);
    Life l = {life, life};
    Collision_circle col = {hitboxRadius};
    Tag tag = ENT_ENEMY;
    Enemy enemy = {type};

    Position_add(&p->position, e, pos);
    Physics_add(&p->physics, e, phy);
    Sprite_add(&p->sprite, e, sprites[graphic]);
    Life_add(&p->life, e, l);
    Collision_circle_add(&p->collision_circle, e, col);
    Tag_add(&p->tag, e, tag);
    Enemy_add(&p->enemy, e, enemy);

    return e;
}

void Enemy_update_all(Pool *p) {
    /**
     * Parcourt tous les ennemis et les tue s'ils n'ont plus de vie
     */
    EnemyManager *em = &p->enemy;

    for (int i = 0; i < em->count; i++) {
        Entity e = em->entity_lookup[i];
        Life *life = Life_get(&p->life, e);
        Tag *tag = Tag_get(&p->tag, e);

        if (life && Life_is_dead(life) && *tag != ENT_BOSS) {
            PlaySound(sfx[SFX_ENEMY_DEATH]);
            pool_kill_entity(p, e);
        }
    }
}