#include "components/enemy.h"
#include "components/common.h"
#include "components/bullet.h"
#include "components/life.h"
#include "components/physics.h"
#include "components/sprite.h"
#include "core/assets.h"
#include "ecs/pool.h"

#include <math.h>
#include <raylib.h>

/* ============================
 *        SPAWN
 * ============================ */

Entity Enemy_spawn(Pool *p, float x, float y, float speed,
                   EnemyType type, MovementPattern pattern, int hp) {

    Entity e = pool_create_entity(p);

    /* Position : angle 90 deg = vers le bas */
    Position pos = {{x, y}, 90.0f};
    Position_add(&p->position, e, pos);

    /* Physique : vitesse vers le bas par défaut */
    Physics phy = Physics_create_speed(speed);
    Physics_add(&p->physics, e, phy);

    /* Sprite : on utilise FAIRY_BLUE_STILL pour l'instant */
    Sprite_add(&p->sprite, e, sprites[FAIRY_BLUE_STILL]);

    /* Tag : c'est un ennemi */
    Tag tag = ENT_ENEMY;
    Tag_add(&p->tag, e, tag);

    /* Vie */
    Life life = {hp, hp};
    Life_add(&p->life, e, life);

    /* Composant Enemy */
    Enemy enemy = {
        .enemyType = type,
        .pattern = pattern,
        .shootTimer = 0,
        .shootRate = 60,      // tire toutes les 60 frames (1 seconde)
        .canShoot = true,
        .patternTimer = 0,
        .patternParam = 50.0f // amplitude par défaut pour le zigzag
    };
    Enemy_add(&p->enemy, e, enemy);

    return e;
}

/* ============================
 *    PATTERNS DE MOUVEMENT
 * ============================ */

static void pattern_straight_down(Physics *phy, Position *pos, Enemy *enemy) {
    /***
     * Pattern simple : l'ennemi descend tout droit
     * La physique gère déjà le mouvement, on ne fait rien de spécial
     */
    (void)phy;
    (void)pos;
    (void)enemy;
    // L'angle est déjà à 90° (vers le bas), la physique fait le reste
}

static void pattern_zigzag(Physics *phy, Position *pos, Enemy *enemy) {
    /***
     * Pattern zigzag : l'ennemi oscille horizontalement en descendant
     * On modifie la vélocité angulaire pour créer le zigzag
     */
    (void)pos;

    enemy->patternTimer += 1.0f;

    // sinf donne une oscillation entre -1 et 1
    // On l'utilise pour changer la direction horizontale
    float oscillation = sinf(enemy->patternTimer * 0.05f) * enemy->patternParam;
    phy->angVel = oscillation * 0.02f;
}

static void pattern_circle_in(Physics *phy, Position *pos, Enemy *enemy) {
    /***
     * Pattern cercle : l'ennemi entre en décrivant un arc
     * Il tourne doucement dans une direction
     */
    (void)pos;

    enemy->patternTimer += 1.0f;

    // Tourne doucement pendant les premières frames, puis se stabilise
    if (enemy->patternTimer < 120) {
        phy->angVel = enemy->patternParam * 0.01f;
    } else {
        phy->angVel = 0;
    }
}

static void Enemy_update_movement(Physics *phy, Position *pos, Enemy *enemy) {
    /***
     * Appelle le bon pattern de mouvement selon le type
     */
    switch (enemy->pattern) {
        case PATTERN_STRAIGHT_DOWN:
            pattern_straight_down(phy, pos, enemy);
            break;
        case PATTERN_ZIGZAG:
            pattern_zigzag(phy, pos, enemy);
            break;
        case PATTERN_CIRCLE_IN:
            pattern_circle_in(phy, pos, enemy);
            break;
    }
}

/* ============================
 *          TIR
 * ============================ */

static void Enemy_update_shoot(Pool *p, Position *pos, Enemy *enemy) {
    /***
     * Gère le tir automatique de l'ennemi
     * Tire une bullet vers le bas toutes les shootRate frames
     */
    if (!enemy->canShoot || enemy->shootRate <= 0) {
        return;
    }

    enemy->shootTimer++;

    if (enemy->shootTimer >= enemy->shootRate) {
        enemy->shootTimer = 0;

        // Tire une bullet vers le bas (angle 90deg)
        Bullet_enemy_spawn(p, pos->pos.x, pos->pos.y, 4.0f, 90.0f, BALL_M_BLACK);
    }
}

/* ============================
 *       UPDATE GLOBAL
 * ============================ */

void Enemy_update_all(Pool *p) {
    /***
     * Met à jour tous les ennemis :
     *  1-Pattern de mouvement
     *  2)Tir automatique
     *  3)Destruction si hors écran (en bas)
     */
    EnemyManager *em = &p->enemy;
    Enemy *enemy;
    Physics *phy;
    Position *pos;
    Entity entity;

    for (int i = 0; i < em->count; i++) {
        enemy = &em->dense[i];
        entity = em->entity_lookup[i];

        pos = Position_get(&p->position, entity);
        phy = Physics_get(&p->physics, entity);

        if (!pos || !phy) continue;

        /* Mouvement */
        Enemy_update_movement(phy, pos, enemy);

        /* Tir */
        Enemy_update_shoot(p, pos, enemy);

        /* Destruction si trop bas */
        if (pos->pos.y > 1200) {
            pool_kill_entity(p, entity);
        }
    }
}