#include "looseLaser.h"
#include "common.h"
#include "pool.h"

bool updateLooseLaser(Loose_laser *laser, Position *pos, Timer *timer){
    /***
    * Update d'un loose laser sur 1 frame 
    * pos est l'ancienne position du laser
    * @return true si le laser doit continuer à exister, false s'il doit être supprimé
    */

    // shifting (decale toutes les nodes d'1 vers l'avant)
    for (int k = MAX_LOOSE_NODES - 1; k > 0; k--) {
        laser->looseNodes[k] = laser->looseNodes[k-1];
    }

    // ajout de la position actuelle en tête
    laser->looseNodes[0] = pos->coord;

    if (laser->looseNodeCount < MAX_LOOSE_NODES) {
        laser->looseNodeCount++;
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
    if(*timer > laser->duration) {
        return false; 
    }

    //le laser continue d'exister
    return true; 
}



void updateAllLooseLasers(Pool *ctx){
    /***
     * Met à jour tout les loose lasers et les ajoute à la kill queue s'ils sont finis
     */
    Loose_laser *laser;
    int lookup;
    Position * pos;
    Timer * timer;

    Loose_laserManager * laserManager = &ctx->looseLaser;
    PositionManager * positionManager = &ctx->position;
    TimerManager * timerManager = &ctx->timer;

    for (int i=0; i < laserManager->count; i++)
    {
        laser = &laserManager->dense[i];
        lookup = laserManager->entity_lookup[i];

        pos = &positionManager->dense[lookup];
        timer = &timerManager->dense[lookup];

        if (!updateLooseLaser(laser, pos, timer)) {
            pool_kill_entity(ctx, lookup);
        }

    }

}

Entity CreateLooseLaser(Pool *ctx, float x, float y, float speed, float angle, float length, float width, Color color, int duration) {
    Entity id = pool_create_entity(ctx);

    Vector2 vect = {x, y};
    Loose_laser loose = {
        Vector2,
        1,
        length,
        width,
        duration
    };

    
    // objects[id].looseTargetLength = length;
    // objects[id].looseWidth = width;
    
    // // Initialise le premier noeud à la position de départ pour éviter un glitch à la frame 1
    // objects[id].looseNodes[0] = (Vector2){x, y};
    // objects[id].looseNodeCount = 1;

    // ObjSprite2D_SetColor(id, color);
    // Obj_SetDelay(id, delay);

    return id;
}