#include "components/looseLaser.h"

static void updateLooseLaser(Loose_laser *laser, Vector2 pos) {
  /***
   * Update d'un loose laser sur 1 frame
   * pos est l'ancienne position du laser
   */

  // shifting (decale toutes les nodes d'1 vers l'avant)
  for (int k = MAX_LOOSE_NODES - 1; k > 0; k--) {
    laser->looseNodes[k] = laser->looseNodes[k - 1];
  }

  // ajout de la position actuelle en tête
  laser->looseNodes[0] = pos;

  if (laser->looseNodeCount < MAX_LOOSE_NODES) {
    laser->looseNodeCount++;
  }

  float currentLen = 0;
  for (int k = 0; k < laser->looseNodeCount - 1; k++) {
    float dist =
        Vector2Distance(laser->looseNodes[k], laser->looseNodes[k + 1]);
    currentLen += dist; // calcul de la taille actuelle du laser

    // Si on dépasse la longueur cible, on coupe ici
    if (currentLen > laser->looseTargetLength) {
      laser->looseNodeCount = k + 2; // On garde ce noeud comme dernier point
      break;
    }
  }
}

void updateAllLooseLasers(Loose_laserManager *laserManager,
                          PositionManager *positionManager) {
  Loose_laser *laser;
  int lookup;
  Vector2 pos;
  for (int i = 0; i < laserManager->count; i++) {
    laser = &laserManager->dense[i];
    lookup = laserManager->entity_lookup[i];
    pos = positionManager->dense[lookup].pos;

    updateLooseLaser(laser, pos);
  }
}
