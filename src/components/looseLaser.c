#include "looseLaser.h"
#include "assets.h"
#include "common.h"
#include "timer.h"
#include "components/bullet.h"
#include "pool.h"

bool loose_laser_update(PositionManager * positionManager, Loose_laser *laser){
    /***
    * Update d'un loose laser sur 1 frame 
    * pos est l'ancienne position du laser
    * @return true si le laser doit continuer à exister, false s'il doit être supprimé
    */

    // shifting (decale toutes les nodes d'1 vers l'avant)
    for (int k = MAX_LOOSE_NODES - 1; k > 0; k--) {
        Entity lookup_k = positionManager->entity_lookup[laser->looseNodes[k]];
        Entity lookup_kmoins1 = positionManager->entity_lookup[laser->looseNodes[k-1]];

        positionManager->dense[lookup_k] = positionManager->dense[lookup_kmoins1]; 
    }

    if (laser->looseNodeCount < MAX_LOOSE_NODES) {
        laser->looseNodeCount++;
        laser->looseNodes[laser->looseNodeCount - 1] = node_create(positionManager, 0, 0);
    }

    float currentLen = 0;
    for (int k = 0; k < laser->looseNodeCount - 1; k++) {
        float dist = Vector2Distance(laser->looseNodes[k], laser->looseNodes[k+1]);
        currentLen += dist; //calcul de la taille actuelle du laser
        
        // Si on dépasse la longueur cible, on coupe ici
        if (currentLen > laser->looseTargetLength) {
            laser->looseNodeCount = k + 2; // On garde ce noeud comme dernier point
            break;
        }
    }

    //le laser doit être détruit
    if(timer_current_time(laser->looseTimer) == laser->looseTimer->nbTime - 1) {
        return false; 
    }

    //le laser continue d'exister
    return true; 
}


void loose_lasers_update_all(Pool *pool){
    /***
     * Met à jour tout les loose lasers et les ajoute à la kill queue s'ils sont finis
     */
    Loose_laser *laser;
    int lookup;
    Position * pos;
    Timer * timer;

    Loose_laserManager * laserManager = &(pool->looseLaser);
    PositionManager * positionManager = &(pool->position);

    for (int i=0; i < laserManager->count; i++)
    {
        laser = &laserManager->dense[i];

        lookup = laserManager->entity_lookup[i];
        pos = &positionManager->dense[lookup];

        if (!updateLooseLaser(laser, pos, timer)) {
            pool_kill_entity(pool, lookup);
        }

    }

}

Entity loose_laser_create(Pool * pool, int x, int y, float speed, float length, float width, Color color, int duration) {
    Entity id = pool_create_entity(pool);

    Entity * nodes = malloc(( sizeof(Vector2) ) * MAX_LOOSE_NODES);
    //création de la tête du laser
    Entity lookup = Bullet_enemy_spawn(pool, x, y, speed, 0, NULL_SPRITE);

    Timer * timer = malloc(sizeof(Timer));
    timer->chrono = 0;
    timer_add_time(timer, duration);

    Loose_laser loose ={
        .looseNodes = nodes,
        .looseNodeCount = 1,
        .looseTargetLength = length,
        .looseWidth = width,
        .looseTimer = timer
    };
    loose.looseNodes[0] = lookup;


    Loose_laser_add(&pool->looseLaser, id, loose);
    Tag_add(&pool->tag, id, ENT_LOOSE_LASER);

    return id;
}



Entity node_create(Pool * pool, float x, float y) {
    Entity id = pool_create_entity(pool);
    Vector2 vect = {x, y};
    Position pos = {.pos = vect, .angle = 0};
    Pos_add(&pool->position, id, pos);
    return id;
}