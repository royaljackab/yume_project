#include "components/common.h"
#include "pool.h"
#include "sprite.h"
#include "screen.h"
#include <raylib.h>

bool Position_is_out_of_bounds(Position *pos) {
    Vector2 p = pos->pos;

    return (p.x < PANEL_LEFT - BOUNDS_X || p.x > PANEL_RIGHT + BOUNDS_X 
     || p.y < PANEL_UP - BOUNDS_Y || p.y > PANEL_DOWN + BOUNDS_Y);
}

void Position_set_position(Position *pos, float x, float y) {
    pos->pos = (Vector2){x,y};
}


bool Tag_in_array(Tag tag, Tag * array, int size) {
    for (int i = 0; i < size; i++) {
        if (tag == array[i]) {
            return true;
        }
    }
    return false;
}

void Owner_bind(Pool *p, Entity OwnerId, Entity ServantId){
    /**
     * Cette fonction permet d'ajouter un owner à une entité. Une entité servante copie la position de son owner.
     */
    Owner_add(&p->owner,ServantId,OwnerId);
}

void Owner_update(Pool *p){
    /**
     * @brief Met à jour la position de toutes les entitées qui ont un owner en copiant celle de leur owner.
     * 
     */
  for(int i=0; i< p->owner.count; i++){
    Entity ownedId = Owner_get_entity(&p->owner, i); //l'id de la propriété
    Entity ownerId = p->owner.dense[i]; //l'id du propriétaire
    if(ownedId == ID_INVALID){
        continue;
    }
    if(ownerId == ID_INVALID){
        pool_kill_entity(p, ownedId);
        continue;
    }

    Position *ownedPos = Position_get(&p->position, ownedId);
    Position *ownerPos = Position_get(&p->position, ownerId);
    if (!ownedPos || !ownerPos) {
        continue;
    }

    ownedPos->pos = ownerPos->pos; //la position de la propriété copie tout le temps celle de l'owner
  }
}
