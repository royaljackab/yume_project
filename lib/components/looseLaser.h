#pragma once

#define MAX_LOOSE_NODES 200


#include "systems/timer.h"
#include "ecs/component.h"
#include <raymath.h>

#include "components/sprite.h"
#include "components/common.h"

typedef struct Pool Pool;

typedef struct {
    /** 
    * Un loose laser est un filament composé de plusieurs nodes.
    * Le node à l'avant se comporte comme une bullet et a une physique.
    * Les autres nodes conservent l'historique des positions du head node, ce qui créé un effet de trainée.
    */

    Entity *looseNodes;                  // Historique des positions
    int looseNodeCount;                  // Nombre actuel de noeuds
    float looseTargetLength;             // Longueur visée (le laser grandit jusqu'à cette taille)
    float looseWidth;                    // Épaisseur du laser
    Timer looseTimer;                    // Timer pour la durée de vie du laser

    Color looseColor;                    // Pour l'affichage uniquement
} Loose_laser;

DEFINE_COMPONENT_MANAGER(Loose_laser, MAX_ENTITIES)

DECLARE_GETTER(Loose_laser, Entity *, looseNodes)
DECLARE_SETTER_GETTER(Loose_laser, int, looseNodeCount)
DECLARE_SETTER_GETTER(Loose_laser, float, looseTargetLength)
DECLARE_SETTER_GETTER(Loose_laser, float, looseWidth)

bool loose_laser_update(Pool *pool, Loose_laser *laser);
void loose_lasers_update_all(Pool *pool);
Entity loose_laser_create(Pool * pool, int x, int y, float speed, float length, float width, int duration, Color color);
Entity node_create(Pool * pool, float x, float y);
void Loose_laser_destroy(Pool * pool, Entity id);
void draw_loose_laser(Loose_laser *laser, PositionManager *positionManager);
void draw_all_loose_lasers(Loose_laserManager *looseManager, PositionManager *positionManager);
