#ifndef OBJ_H
#define OBJ_H

#include "raylib.h"
#include "raymath.h"

#include "sprite.h"

#define NO_CHANGE -9999999
#define NO_LIMIT 9999999
#define MAX_PATTERNS 8
#define MAX_LOOSE_NODES 200

typedef int ObjID;

typedef enum {
    ID_INVALID = -1,
    OBJ_PLAYER,
    OBJ_ENEMY,
    OBJ_BOSS,
    OBJ_ENEMY_SHOT,
    OBJ_PLAYER_SHOT,
    OBJ_ENEMY_LASER,
    OBJ_PLAYER_LASER,
    OBJ_LOOSE_LASER
} ObjType;

typedef struct {
    int delay;
    float speed;
    float angle;
    
    float accel;
    float angVel;
    float maxSpd;
    Vector2 force;
} MovePattern;

typedef struct {
    bool active;
    ObjType type;

    Vector2 pos;
    float speed;
    float angle; //en degrés
    float accel;
    float maxSpd;
    float angVel;
    Vector2 force;

    float life;
    float maxLife;
    int damage;

    Sprite sprite;

    bool movingToDest;
    Vector2 destPos;

    int delay;
    int timer;

    //straight Laser
    float laserLength;
    float laserWidth;                 //taille actuelle affichée
    float laserMaxWidth;              //taille quand le laser s'élargit au max
    float intersectionWidth;          // largeur hitbox
    float invalidLengthBase;
    float invalidLengthTip;
    int laserState;                   // 0:warning 1:actif 2:inactif //peut etre inutile...
    int warningTimer;
    int growingTimer;
    int laserDuration;

    //loose laser
    Vector2 looseNodes[MAX_LOOSE_NODES]; // Historique des positions
    int looseNodeCount;                  // Nombre actuel de noeuds
    float looseTargetLength;             // Longueur visée (le laser grandit jusqu'à cette taille)
    float looseWidth;                    // Épaisseur du laser


    // File de patterns
    MovePattern patterns[MAX_PATTERNS];
    int patternCount;

    // Flags
    bool disappearOnHit; // L'objet disparaît lorsque collision avec un ennemi
    bool disappearOnDeath; // L'objet disparaît lorsque life=0
} Object;

ObjID Obj_Create (ObjType type);
void Obj_Delete  (ObjID* id);
void Obj_SetDelay(ObjID id, int delay);

void ObjMove_SetPosition   (ObjID id, float x, float y);
void ObjMove_SetSpeed      (ObjID id, float speed);
void ObjMove_SetAngle      (ObjID id, float angle);
void ObjMove_SetDestAtSpeed(ObjID id, float x, float y, float speed);
void ObjMove_SetDestAtFrame(ObjID id, float x, float y, int nbFrames);
void ObjMove_SetAccel      (ObjID id, float acc, float maxSpd);
void ObjMove_AddPattern    (ObjID id, int frameDelay, float speed, float angle, float accel, float maxSpd, float angVel);
void ObjMove_SetForce      (ObjID id, float x, float y);

void ObjLaser_SetLength           (ObjID id, float length);
void ObjLaser_SetMaxWidth         (ObjID id, float maxWidth);
void ObjLaser_SetTimers           (ObjID id, int warning, int growing, int duration);
void ObjLaser_SetIntersectionWidth(ObjID id, int intersectionWidth);
void ObjLaser_SetInvalidLength    (ObjID id, int ratioBase, int ratioTip);
void ObjLaser_SetGrowingTime      (ObjID id, int growing);

void ObjLoose_SetLengthWidth(ObjID id, float length, float width);

void UpdateObjects();

void Obj_SetTexture                (ObjID id, int textureID);
void ObjSprite2D_SetSourceRect     (ObjID id, float x, float y, float width, float height);
void ObjSprite2D_SetCenter         (ObjID id, float x, float y);
void ObjSprite2D_SetRotation       (ObjID id, float rotation);
void ObjSprite2D_SetScale          (ObjID id, float x, float y);
void ObjSprite2D_SetColor          (ObjID id, Color color);
void ObjSprite2D_SetAnimation      (ObjID id, int frameCount, int delay);
void ObjSprite2D_SetCollisionToShot(ObjID id, float radius);

void UpdateAnimations();
void DrawObjects();
void UpdateCollisions();

#endif