#include "looseLaser.h"
#include "assets.h"
#include "common.h"
#include "timer.h"
#include "components/bullet.h"
#include "pool.h"

#include "stdio.h"

///TODO: ajouter des hitbox aux nodes

bool loose_laser_update(Pool *pool, Loose_laser *laser){
    /***
    * Update d'un loose laser sur 1 frame 
    * @return true si le laser doit continuer à exister, false s'il doit être supprimé
    */
    PositionManager * positionManager = &(pool->position);

    if (laser->looseNodeCount < MAX_LOOSE_NODES) {
        Vector2 lastCoord;
        //on doit placer le node nouvellement créé à la position du précédent dernier node
        lastCoord = Position_get(positionManager,laser->looseNodes[laser->looseNodeCount - 1])->pos;
        
        laser->looseNodes[laser->looseNodeCount] = node_create(pool, lastCoord.x, lastCoord.y, Loose_laser_get_looseWidth(laser) / 2.0f);
        laser->looseNodeCount++;
    }
    // printf("nb_node: %d\n",laser->looseNodeCount);

    // shifting (Chaque node copie la pos du node devant lui)
    // Le node 0 est le plus à l'avant. Les autres nodes copient ses précédentes positions
    for (int k = laser->looseNodeCount - 1; k > 0; k--) {
        *Position_get(positionManager,laser->looseNodes[k]) = *Position_get(positionManager,laser->looseNodes[k-1]); 
    }



    float currentLen = 0;
    for (int k = 0; k < laser->looseNodeCount - 1; k++) {
        float dist = Vector2Distance(Position_get(positionManager,laser->looseNodes[k])->pos, Position_get(positionManager,laser->looseNodes[k+1])->pos);
        currentLen += dist; //calcul de la taille actuelle du laser
        
        // Si on dépasse la longueur cible, on coupe ici
        if (currentLen > laser->looseTargetLength) {
            for (int j = k + 2; j < laser->looseNodeCount; j++) {
                pool_kill_entity(pool, laser->looseNodes[j]);
            }
            laser->looseNodeCount = k + 2; // On garde ce noeud comme dernier point
            break;
        }
    }

    //mise a jour du timer
    ///TODO: Je ne sais pas comment gêrer les loops
    laser->looseTimer.chrono++;


    //le laser doit être détruit si le timer a atteint son dernier temps (seul temps: duration)
    if(timer_current_time(&laser->looseTimer) == 0) {
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

    Loose_laserManager * laserManager = &(pool->looseLaser);

    for (int i=0; i < laserManager->count; i++)
    {
        laser = &(laserManager->dense[i]);
        if (!loose_laser_update(pool, laser)) {
            pool_kill_entity(pool, laserManager->entity_lookup[i]);
        }
    }

}

Entity loose_laser_create(Pool * pool, int x, int y, float speed, float length, float width, int duration, Color color) {
    /***
     * Créé un loose laser dans la pool
     */
    Entity id = pool_create_entity(pool);
    Timer timer;
    Entity * nodes = malloc(sizeof(Entity) * MAX_LOOSE_NODES);

    //création de la tête du laser
    Entity bulletId = Bullet_enemy_spawn(pool, x, y, speed, 0, NULL_SPRITE);

    //valeurs initiales
    timer.chrono = 0;
    timer.nbTime = 0;
    timer_add_time(&timer, duration);
    Loose_laser loose ={
        .looseNodes = nodes,
        .looseNodeCount = 1, 
        .looseTargetLength = length,
        .looseWidth = width,
        .looseTimer = timer,
        .looseColor = color
    };
    loose.looseNodes[0] = bulletId;

    Loose_laser_add(&pool->looseLaser, id, loose);
    Tag_add(&pool->tag, id, ENT_LOOSE_LASER);

    return id;
}



Entity node_create(Pool * pool, float x, float y, float radius) {
    /***
     * Créé un node de loose laser dans la pool
     */
    Entity id = pool_create_entity(pool);
    Vector2 vect = {x, y};
    Position pos = {.pos = vect, .angle = 0};
    Position_add(&pool->position, id, pos);
    Collision_circle circle = {.radius = radius};
    Collision_circle_add(&pool->collision_circle, id, circle);
    return id;
}

void Loose_laser_destroy(Pool * pool, Entity id){
    /***
     * Détruit un loose_laser dans la pool (y compris on libérant tout ce qui le compose, contrairement à loose_laser_remove)
     */


    Loose_laser * laser = Loose_laser_get(&pool->looseLaser,id);

    if(!laser){
        return;
    }

    for(int i = 0; i < laser->looseNodeCount; i++){
        pool_kill_entity(pool, laser->looseNodes[i]);
    }
    free(laser->looseNodes);
    Loose_laser_remove(&pool->looseLaser, id);
}

void draw_loose_laser(Loose_laser *laser, PositionManager *positionManager){
    /***
     * Affiche un loose laser
     */
    if (laser->looseNodeCount < 2) return;

    // On a besoin de 2 sommets par noeud (gauche et droite)
    int count = laser->looseNodeCount;
    
    float w = laser->looseWidth / 2.0f;
    Color col = laser->looseColor;

    int nbNodes = laser->looseNodeCount;

    // On parcourt les segments
    for (int k = 0; k < count - 1; k++) {
        float wp = k * (nbNodes - k) * (4.0/(nbNodes*nbNodes*1.0)) * w;

        Vector2 p1 = Position_get(positionManager,laser->looseNodes[k])->pos;
        Vector2 p2 = Position_get(positionManager,laser->looseNodes[k+1])->pos;
        
        // Calcul de la normale au segment
        Vector2 diff = Vector2Subtract(p1, p2);
        Vector2 normal = Vector2Normalize((Vector2){ -diff.y, diff.x }); // Perpendiculaire
        
        // Points du quadrilatère (P1 Left, P1 Right, P2 Left, P2 Right)
        Vector2 p1L = Vector2Add(p1, Vector2Scale(normal, wp));
        Vector2 p1R = Vector2Subtract(p1, Vector2Scale(normal, wp));
        Vector2 p2L = Vector2Add(p2, Vector2Scale(normal, wp));
        Vector2 p2R = Vector2Subtract(p2, Vector2Scale(normal, wp));

        // Dessin de 2 triangles pour former le segment
        DrawTriangle(p2R, p2L, p1L, col);
        DrawTriangle(p1R, p2R, p1L, col);
        
        // Optionnel: Ajouter des cercles aux jointures pour lisser les angles
        DrawCircleV(p1, wp, col);
    }
}

void draw_all_loose_lasers(Loose_laserManager *looseManager, PositionManager * positionManager) {
  /**
   * Affiche tous les loose lasers actifs
   */
  Loose_laser *laser;
  for (int i=0; i < looseManager->count; i++) {
      laser = &looseManager->dense[i];
      draw_loose_laser(laser, positionManager);
  }
}