#include "../lib/task.h"
#include "../lib/test_task.h"

#define bossX objects[bossID].pos.x
#define bossY objects[bossID].pos.y

ObjID bossID;

void initialize() {
    bossID = Obj_Create(OBJ_BOSS);

    //texture du boss
    Obj_SetTexture(bossID, BOSS_SPRITESHEET);
    ObjSprite2D_SetSourceRect(bossID, 16,187,512,84);
    ObjSprite2D_SetAnimation(bossID, 8, 5);
    ObjSprite2D_SetCollisionToShot(bossID, 20);

    ObjMove_SetPosition(bossID, -20, -20);
} 

Define_Static_Task(fireRing, NO_PARAMS);
    float angleT = GetRandomValue(0,360);
    for (int i = 0; i < 48; i++) {
        CreateShot(bossX,bossY, 4.5, angleT, BALL_M_BLACK, 0);
        angleT += 360.0/48;
    }
End_Task;

Define_Static_Task(fireLaser, PARAMS(int dir));
    ObjID obj = CreateLaser(bossX,bossY,180 + 105*dir,400,15,120, BALL_M_BLACK, 30);
    ObjMove_AddPattern(obj, 30, NO_CHANGE, NO_CHANGE, NO_CHANGE, NO_CHANGE, 0.5 * dir);
End_Task;

Define_Static_Task(loser, NO_PARAMS);
    ObjID obj = CreateLooseLaser(bossX,bossY,3,45,200,10,YELLOW,0);
    ObjMove_AddPattern(obj, 30, NO_CHANGE, NO_CHANGE, NO_CHANGE, NO_CHANGE, 3);
    ObjMove_AddPattern(obj, 60, NO_CHANGE, NO_CHANGE, -5, 0, 0);
    ObjMove_AddPattern(obj, 120, NO_CHANGE, NO_CHANGE, 5, 7, 0);
End_Task;

Define_Static_Task(snakeRing, PARAMS(Color color));
    float angleT=0;
    for(int i=0; i<5; i++) {
        ObjID laser = CreateLooseLaser(bossX, bossY, 3, angleT, 200, 50, color, 0);
        ObjMove_AddPattern(laser, 40, NO_CHANGE, angleT+20, NO_CHANGE, NO_CHANGE, NO_CHANGE);
        angleT += 360.0/5;
    }
End_Task;

Define_Static_Task(movement, NO_PARAMS);
    while(true) {
        ObjMove_SetDestAtFrame(bossID, GetRandomValue( 30, 500), 200 + GetRandomValue(-20,50), 60);
        wait(360);
    }
End_Task;

Define_Task(moonlight_task, NO_PARAMS, int count; void *ringState; void *laserState; void *moveState; void *loserState;);
    ctx->ringState = NULL; ctx->moveState = NULL; ctx->laserState = NULL; ctx->loserState = NULL;
    ctx->count = 0;
    ObjMove_SetDestAtFrame(bossID, 300, 200, 60);
    wait(80);

    while(1) {
        movement(&ctx->moveState);
        if(ctx->count % 10 == 0) {
            fireRing(&ctx->ringState);
        }
        if(ctx->count % 360 == 90) {
            fireLaser(&ctx->laserState, 1);
            fireLaser(&ctx->laserState, -1);
            snakeRing(&ctx->loserState, GREEN);
        }
        ctx->count++;
        yield;
    }
End_Task;
