//TODO: Eliminer le fichier décadent obj.c et intégrer les fonctions dans le pool et/ou les systèmes concernés


#include "../lib/obj.h"
#include "../lib/globals.h"
#include <stdio.h>

#define o objects[i]

Object objects[MAX_OBJECTS];

ObjID Obj_Create(ObjType type) {
    for (int i=0; i<MAX_OBJECTS; i++) {
        if(!objects[i].active) {
            objects[i].active = true;
            objects[i].type = type;
            objects[i].speed = 0;
            objects[i].sprite.color = WHITE;
            objects[i].maxLife = 100;
            objects[i].life = 100;
            objects[i].timer = 0;
            objects[i].delay = 0;
            objects[i].accel = 0;
            objects[i].angVel = 0;
            objects[i].maxSpd = NO_LIMIT;
            objects[i].patternCount = 0;
            objects[i].force = (Vector2){0,0};
            objects[i].disappearOnHit = 1;
            return i; //i devient ObjID
        }
    }
    
    //Aucune place disponible
    return ID_INVALID;
}

/* Fonctions statiques */
static void ShiftPatterns(Object* obj) {
    for (int i=0; i < obj->patternCount - 1; i++) {
        obj->patterns[i] = obj->patterns[i+1];
    }
    obj->patternCount--;
}

static void ApplyMoveParams(Object* obj, MovePattern* p) {
    if (p->speed != NO_CHANGE) obj->speed = p->speed;
    if (p->angle != NO_CHANGE) obj->angle = p->angle;
    if (p->accel != NO_CHANGE) obj->accel = p->accel;
    if (p->angVel != NO_CHANGE) obj->angVel = p->angVel;
    if (p->maxSpd != NO_CHANGE) obj->maxSpd = p->maxSpd;
}

static bool IsOutOfDrawBounds(Vector2 pos, Sprite sprite) {
    /***
     * Retourne si le sprite à la position pos est hors de la fenêtre de dessin
     */

    //grossier
    return (pos.x + sprite.srcRect.width < PANEL_LEFT || pos.x - sprite.srcRect.width > PANEL_LEFT + PANEL_WIDTH 
        || pos.y + sprite.srcRect.height < PANEL_UP || pos.y - sprite.srcRect.height > PANEL_UP + PANEL_HEIGHT);
        
}

static bool IsOutOfBounds(Vector2 pos) {
    /***
     * Retourne si l'objet est hors limites ou pas
     */

     return (pos.x < - DRAW_MARGIN || pos.x > PANEL_WIDTH + PANEL_LEFT + DRAW_MARGIN
        || pos.y < -DRAW_MARGIN || pos.y > PANEL_UP + PANEL_HEIGHT + DRAW_MARGIN);
}

/* Fonctions globales */
void ObjMove_AddPattern(ObjID id, int frameDelay, float speed, float angle, float accel, float maxSpd, float angVel) {
    if (id==ID_INVALID) return;
    if (objects[id].patternCount >= MAX_PATTERNS) return;

    int i = objects[id].patternCount;
    objects[id].patterns[i].delay = frameDelay;
    objects[id].patterns[i].speed = speed;
    objects[id].patterns[i].angle = angle;
    objects[id].patterns[i].maxSpd = maxSpd;
    objects[id].patterns[i].accel = accel;
    objects[id].patterns[i].angVel = angVel;

    objects[id].patternCount++;
}

void Obj_Delete(ObjID* id) {
    if(*id==ID_INVALID) return;
    objects[*id].active=false;
    *id = ID_INVALID;
}

void Obj_SetDelay(ObjID id, int delay) {
    if(id==ID_INVALID) return;
    objects[id].delay = delay;
}

void ObjMove_SetPosition(ObjID id, float x, float y) {
    if(id==ID_INVALID) return;
    objects[id].pos = (Vector2){x,y};
}

void ObjMove_SetSpeed(ObjID id, float speed) {
    if(id == ID_INVALID) return;
    objects[id].speed = speed;
}

void ObjMove_SetAngle(ObjID id, float angle) {
    if(id == ID_INVALID) return;
    objects[id].angle = angle;
}

void ObjMove_SetAccel(ObjID id, float acc, float maxSpd) {
    if(id==ID_INVALID) return;
    objects[id].accel = acc;
    objects[id].maxSpd = maxSpd;
}

void ObjMove_SetDestAtSpeed(ObjID id, float x, float y, float speed) {
    if(id==ID_INVALID) return;
    objects[id].movingToDest = true;
    objects[id].destPos = (Vector2){x,y};

    objects[id].speed = speed;
    objects[id].angle = atan2f(y - objects[id].pos.y,x - objects[id].pos.x) * (180.0f/PI);
}

void ObjMove_SetDestAtFrame(ObjID id, float x, float y, int nbFrames) {
    if(id==ID_INVALID) return;
    float distance = Vector2Distance(objects[id].pos, (Vector2){x,y});
    float speed = distance / nbFrames;
    ObjMove_SetDestAtSpeed(id, x, y, speed);
}

void ObjMove_SetForce(ObjID id, float x, float y) {
    if(id==ID_INVALID) return;
    objects[id].force = (Vector2){x,y};
}

void ObjLaser_SetLength(ObjID id, float length) {
    if(id==ID_INVALID) return;
    objects[id].laserLength = length;
}

void ObjLaser_SetMaxWidth(ObjID id, float maxWidth) {
    if(id==ID_INVALID) return;
    objects[id].laserMaxWidth = maxWidth;
}

void ObjLaser_SetTimers(ObjID id, int warning, int growing, int duration) {
    if(id==ID_INVALID) return;
    if (warning != NO_CHANGE) objects[id].warningTimer = warning;
    if (growing != NO_CHANGE) objects[id].growingTimer = growing;
    if (duration != NO_CHANGE) objects[id].laserDuration = duration;
}

void ObjLaser_SetIntersectionWidth(ObjID id, int intersectionWidth) {
    if(id==ID_INVALID) return;
    objects[id].intersectionWidth = intersectionWidth;
}

void ObjLaser_SetInvalidLength(ObjID id, int ratioBase, int ratioTip) {
    if(id==ID_INVALID) return;
    objects[id].invalidLengthBase = ratioBase;
    objects[id].invalidLengthTip = ratioTip;
}   

void ObjLaser_SetGrowingTime(ObjID id, int growing) {
    ObjLaser_SetTimers(id, NO_CHANGE, growing, NO_CHANGE);
}

void ObjLoose_SetLengthWidth(ObjID id, float length, float width) {
    if(id==ID_INVALID) return;
    objects[id].looseTargetLength = length;
    objects[id].looseWidth = width;
}

void ObjEnemy_SetLife(ObjID id, float hp) {
    if(id==ID_INVALID) return;
    objects[id].maxLife = hp;
    objects[id].life = hp;
}

void ObjEnemy_AddLife(ObjID id, float hp) {
    if(id==ID_INVALID) return;
    objects[id].life -= hp;
}

void UpdateObjects() {
    for (int i=0; i<MAX_OBJECTS; i++) {
        if(!objects[i].active) continue;

        if( (objects[i].type != OBJ_LOOSE_LASER && IsOutOfBounds(objects[i].pos)) || (objects[i].disappearOnDeath && objects[i].life <= 0) )
            objects[i].active=false;
        if (objects[i].type == OBJ_LOOSE_LASER && IsOutOfBounds(objects[i].looseNodes[objects[i].looseNodeCount-1]))
            objects[i].active=false;
        objects[i].timer++;

        //Gestion patterns
        if(objects[i].patternCount > 0) {
            MovePattern* currPat = objects[i].patterns;
            currPat->delay--;

            if (currPat->delay <= 0) {
                ApplyMoveParams(&objects[i], currPat);
                ShiftPatterns(&objects[i]);
            }
        }

        //Physique
        objects[i].angle += objects[i].angVel;

        //Acceleration
        if (objects[i].accel != 0) {
            objects[i].speed += objects[i].accel;
        }
        if (objects[i].maxSpd != NO_LIMIT) {
            if(objects[i].accel > 0 && objects[i].speed > objects[i].maxSpd) objects[i].speed = objects[i].maxSpd;
            else if(objects[i].accel < 0 && objects[i].speed < objects[i].maxSpd) objects[i].speed = objects[i].maxSpd;
        }

        //Force
        if(objects[i].force.x != 0 || objects[i].force.y != 0) {
            float rad = objects[i].angle * DEG2RAD;

            float vx = cosf(rad) * objects[i].speed;
            float vy = sinf(rad) * objects[i].speed;

            vx += objects[i].force.x;
            vy += objects[i].force.y;

            objects[i].speed = sqrtf(vx*vx + vy*vy);
            objects[i].angle = atan2f(vy,vx) * RAD2DEG;
        }

        if(objects[i].movingToDest) {
            float distRemaining = Vector2Distance(objects[i].pos, objects[i].destPos);

            if (distRemaining <= objects[i].speed) {
                objects[i].pos = objects[i].destPos;
                objects[i].speed = 0;
                objects[i].movingToDest = false;
                continue;
            }
        }

        //Direction
        float rad = objects[i].angle * DEG2RAD;
        Vector2 velocity = Vector2Scale((Vector2){cosf(rad), sinf(rad)}, objects[i].speed);
        objects[i].pos = Vector2Add(objects[i].pos, velocity);

        //Straight laser
        if(objects[i].type == OBJ_ENEMY_LASER || objects[i].type == OBJ_PLAYER_LASER) {
            int warning = objects[i].warningTimer;
            int growing = objects[i].growingTimer;
            int duration = objects[i].laserDuration;

            if(objects[i].timer < warning) {
                objects[i].laserWidth = 2;
                objects[i].laserState = 0;
            }
            else if(objects[i].timer < growing + warning) {
                objects[i].laserWidth += objects[i].laserMaxWidth / objects[i].growingTimer;
                objects[i].laserState = 1;
            }
            else if(objects[i].timer < growing + warning + duration) {
                objects[i].laserWidth = objects[i].laserMaxWidth;
                objects[i].laserState = 1;
            }
            else {
                objects[i].laserWidth -= objects[i].laserMaxWidth / objects[i].growingTimer;
                if(objects[i].laserWidth <= 0) objects[i].active = false;
            }
        }

        //loose laser
        if(objects[i].type == OBJ_LOOSE_LASER) {
            // shifting
            for (int k = MAX_LOOSE_NODES - 1; k > 0; k--) {
                objects[i].looseNodes[k] = objects[i].looseNodes[k-1];
            }
            // ajout de la position actuelle en tête
            objects[i].looseNodes[0] = objects[i].pos;

            if (objects[i].looseNodeCount < MAX_LOOSE_NODES) {
                objects[i].looseNodeCount++;
            }

            float currentLen = 0;
            for (int k = 0; k < objects[i].looseNodeCount - 1; k++) {
                float dist = Vector2Distance(objects[i].looseNodes[k], objects[i].looseNodes[k+1]);
                currentLen += dist;
                
                // Si on dépasse la longueur cible, on coupe ici
                if (currentLen > objects[i].looseTargetLength) {
                    objects[i].looseNodeCount = k + 2; // On garde ce noeud comme dernier point
                    break;
                }
            }
        }
    }
}


void Obj_SetTexture(ObjID id, int textureID) {
    if(id == ID_INVALID) return;
    SetTexture(&objects[id].sprite, textureID);
}

void ObjSprite2D_SetSourceRect(ObjID id, float x, float y, float width, float height) {
    if(id==ID_INVALID) return;
    SetSourceRect(&objects[id].sprite, x, y, width, height);
}

void ObjSprite2D_SetCenter(ObjID id, float x, float y) {
    if(id==ID_INVALID) return;
    SetCenter(&objects[id].sprite, x,y);
}

void ObjSprite2D_SetRotation(ObjID id, float rotation) {
    if(id==ID_INVALID) return;
    SetRotation(&objects[id].sprite, rotation);
}

void ObjSprite2D_SetScale(ObjID id, float x, float y) {
    if(id==ID_INVALID) return;
    SetScale(&objects[id].sprite,x,y);
}

void ObjSprite2D_SetColor(ObjID id, Color color) {
    if(id==ID_INVALID) return;
    SetColor(&objects[id].sprite, color);
}

void ObjSprite2D_SetAnimation(ObjID id, int frameCount, int delay) {
    if(id==ID_INVALID) return;
    SetAnimation(&objects[id].sprite, frameCount, delay);
}

void ObjSprite2D_SetCollisionToShot(ObjID id, float radius) {
    if(id==ID_INVALID) return;
    SetCollisionRadius(&objects[id].sprite, radius);
}

void UpdateAnimations() {
    for(int i=0; i<MAX_OBJECTS; i++) {
        if (objects[i].active && objects[i].sprite.isAnimated) {
            UpdateAnimation(&objects[i].sprite);
        }
    }
}

void DrawObjects() {
    for(int i=0; i<MAX_OBJECTS; i++) {
        if(objects[i].active && objects[i].timer >= objects[i].delay && !IsOutOfDrawBounds(objects[i].pos, objects[i].sprite)) {

            //straight laser
            if (objects[i].type == OBJ_ENEMY_LASER || objects[i].type == OBJ_PLAYER_LASER) {
                int textureID = objects[i].sprite.textureID;

                Rectangle source = objects[i].sprite.srcRect;

                Rectangle dest = {
                    objects[i].pos.x,
                    objects[i].pos.y,
                    objects[i].laserWidth,
                    objects[i].laserLength
                };

                Vector2 origin = {objects[i].laserWidth/2.0, 0};

                DrawTexturePro(textures[textureID], source, dest, origin, objects[i].angle, objects[i].sprite.color);
                continue;
            }

            // Affichage LOOSE LASER
            if (objects[i].type == OBJ_LOOSE_LASER) {
                if (objects[i].looseNodeCount < 2) continue;

                // On a besoin de 2 sommets par noeud (gauche et droite)
                int count = objects[i].looseNodeCount;
                
                float w = objects[i].looseWidth / 2.0f;
                Color col = objects[i].sprite.color;

                int nbNodes = objects[i].looseNodeCount;

                // On parcourt les segments
                for (int k = 0; k < count - 1; k++) {
                    float wp = k * (nbNodes - k) * (4.0/(nbNodes*nbNodes*1.0)) * w;

                    Vector2 p1 = objects[i].looseNodes[k];
                    Vector2 p2 = objects[i].looseNodes[k+1];
                    
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
                
                continue; 
            }

            
            if(objects[i].type != OBJ_BOSS) objects[i].sprite.rotation = objects[i].angle;
            DrawSprite(objects[i].sprite, objects[i].pos);
        }
    }
}

void UpdateCollisions() {
    for(int j=0; j<MAX_OBJECTS; j++) {
        if (!objects[j].active) continue;

        switch(objects[j].type) {
            case OBJ_ENEMY:
            case OBJ_BOSS:
                for (int i=0; i<MAX_OBJECTS; i++) {     
                    if (!objects[i].active) continue;              
                    if (objects[i].type != OBJ_PLAYER_SHOT && objects[i].type != OBJ_PLAYER_LASER) continue;

                    if (CheckCollisionCircles(objects[i].pos, objects[i].sprite.hitboxRadius, objects[j].pos, objects[j].sprite.hitboxRadius) == true) {
                        objects[j].life -= objects[i].damage;
                        if (objects[i].disappearOnHit == 1) objects[i].active = false;
                    }
                }
                break;
            default: break;
        }
    }
}


