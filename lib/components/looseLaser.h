#pragma once
#define MAX_LOOSE_NODES 200

#include "component.h"

typedef struct {
    //loose laser
    Vector2 looseNodes[MAX_LOOSE_NODES]; // Historique des positions
    int looseNodeCount;                  // Nombre actuel de noeuds
    float looseTargetLength;             // Longueur visée (le laser grandit jusqu'à cette taille)
    float looseWidth;                    // Épaisseur du laser

} Loose_laser;


DEFINE_COMPONENT_MANAGER(Loose_laser, MAX_ENTITIES)

DECLARE_GETTER(Loose_laser, Vector2 *, looseNodes)
DECLARE_SETTER_GETTER(Loose_laser, int, looseNodeCount)
DECLARE_SETTER_GETTER(Loose_laser, float, looseTargetLength)
DECLARE_SETTER_GETTER(Loose_laser, float, looseWidth)

void updateLooseLaser(Loose_laser laser, Vector2 pos);
void updateAllLooseLasers(Loose_laserManager * laserManager, PositionManager * positionManager);
