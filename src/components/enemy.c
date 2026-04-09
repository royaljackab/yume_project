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
#include "components/collision_entity.h"


#include <stdio.h>
#include "content/assets.h"
#include "ecs/pool.h"
#include "obj.h"
#include "systems/score.h"

Entity Enemy_spawn(Pool *p, float x, float y, float speed, float angle,
                   int life, float hitboxRadius,
                   int score, SpriteID graphic) {

    Entity e = pool_create_entity(p);

    Position pos = {{x, y}, angle};
    Physics phy = Physics_create_speed(speed);
    Life l = {life, life};
    Collision_circle col = {hitboxRadius};
    Tag tag = ENT_ENEMY;
    Enemy enemy = {score};

    Position_add(&p->position, e, pos);
    Physics_add(&p->physics, e, phy);
    Sprite_add(&p->sprite, e, sprites[graphic]);
    Life_add(&p->life, e, l);
    Collision_circle_add(&p->collision_circle, e, col);
    Tag_add(&p->tag, e, tag);
    Enemy_add(&p->enemy, e, enemy);

    obj_SetRenderPriority(p, e, RENDER_PRIO_ENEMY);

    return e;
}

Entity Enemy_spawn_score_decrease(Pool *p, float x, float y, float speed, float angle, float hitboxRadius, int score, SpriteID graphic) {
    Entity e = Enemy_spawn(p, x, y, speed, angle, 1, hitboxRadius, score, graphic);
    Entity_add_flag(p, e, FLAG_DECREASE_SCORE);
    return e;    
}


void Enemy_update_all(Pool *p, ScoreSystem *scoreS) {
    /**
     * Parcourt tous les ennemis.
     * - les tue s'ils n'ont plus de vie
     * - ajoute du score au joueur si c'est le cas
     * - leur inflige des dégats s'ils sont en collision avec une balle du joueur
     */
    EnemyManager *em = &p->enemy;
    flagList projectileFlag = {.flags = (FlagType[]){FLAG_PROJECTILE_PLAYER}, .size = 1};
    
    //ce flag est plus précis que "FLAG_PROJECTILE_PLAYER": il n'inclut pas les lasers. Utile pour la destruction après impact.
    flagList bulletFlag = {.flags = (FlagType[]){FLAG_BULLET_PLAYER}, .size = 1}; 
    
    //On met toutes les entités en collision avec l'ennemi courant dans ce tableau
    Entity collision[MAX_COLLISIONS];
    int nbCollisions;

    for (int i = 0; i < em->count; i++) {
        Entity e = Enemy_get_entity(&p->enemy,i);
        Life *life = Life_get(&p->life, e);
        
        //mort de l'ennemi
        if (life && Life_is_dead(life) && obj_GetTag(p, e) != ENT_BOSS) {
            PlaySound(sfx[SFX_ENEMY_DEATH]);
            pool_kill_entity(p, e);
            if (!Entity_has_flag(p, e, FLAG_DECREASE_SCORE)){
                score_decrease(scoreS, Enemy_get(&p->enemy, e)->score);
            }
            else
                score_increase(scoreS, Enemy_get(&p->enemy, e)->score);
        }

        //collisions avec l'ennemi
        nbCollisions = 0;
        Entity_find_hitters(p, e, &projectileFlag, collision, &nbCollisions);
        for(int j = 0; j < nbCollisions; j++){

            //perte de pv
            if (!Entity_has_flag(p, e, FLAG_INVINCIBLE) && life){
                Entity playerId = Player_get_playerID(p);
                Life_damage(life, Weapon_get(&p->weapon,playerId)->attack_damage);
            }

            //détruire la bullet du joueur après impact
            if(Entity_has_flag_in_list(p, collision[j], &bulletFlag)){
                pool_kill_entity(p, collision[j]);
                score_increase(scoreS, scoreS->scoreOnHit);
            }
        }

    }
}