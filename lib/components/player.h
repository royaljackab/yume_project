#pragma once

#include "component.h"
#include "collision_circle.h"
#include "common.h"


#define INITIAL_PLAYER_LIVES 3

typedef struct GameContext GameContext;
typedef struct Pool Pool;

typedef enum {
    TEST_PLAYER
} PlayerName;

typedef struct Player {

    int nbBombs;
    int bombs;

    float speed;
    float focusSpeed;
    Entity hitboxId;

    PlayerName name;
} Player;

typedef enum {
    DEFAULT_PATTERN
} PatternType;

typedef struct Weapon {
    int fire_rate;
    int cooldown;
    PatternType pattern;
} Weapon;

DEFINE_COMPONENT_MANAGER(Player, 1);
DEFINE_COMPONENT_MANAGER(Weapon, 1);

DECLARE_SETTER_GETTER(Player, int, nbBombs);
DECLARE_SETTER_GETTER(Player, int, bombs);
DECLARE_SETTER_GETTER(Player, float, speed);
DECLARE_SETTER_GETTER(Player, float, focusSpeed);
DECLARE_SETTER_GETTER(Player, Entity, hitboxId);


extern void Player_start(Pool *p, PlayerName name, PatternType type);
extern void Player_update(GameContext *ctx);

extern void teleport_to_player_spawn(Pool *p, Entity e);
extern bool Damage_player(Pool *p, Entity player);
