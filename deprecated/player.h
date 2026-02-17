#ifndef PLAYER_H
#define PLAYER_H

#include "sprite.h"
#include "game_state.h"
#include "bullet_shot.h"

typedef struct {
    Sprite sprite;

    Vector2 pos;
    float speed;
    float focusSpeed;

    int nbLife;
    int life;
    int nbBombs;
    int bomb;

    int playerType;
} Player;

enum {
    REIMU_A,
    REIMU_B,
    MARISA_A,
    MARISA_B
};

enum {
    REIMU_IDLE,
    REIMU_LEFT,
    REIMU_RIGHT,

    MARISA_IDLE,
    MARISA_LEFT,
    MARISA_RIGHT,

    HITBOX
};

extern Player player;
extern Sprite playerSprites[10];
extern Player characters[4];

void Load_PlayerSprites   ();
void Load_Players         ();
void InitPlayer           (int playerType);
void UpdatePlayer         (GameContext *ctx);
void DrawPlayer           ();
void UpdateAnimationPlayer();


#endif
