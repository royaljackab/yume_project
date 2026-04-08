#pragma once

#include "component.h"
#include "collision_circle.h"
#include "common.h"
#include "systems/score.h"

#define INITIAL_PLAYER_LIVES 4

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

    Entity hitboxSpriteId;
    Entity GrazeSpriteId;
    
    int grazeRadius;

    PlayerName name;
} Player;

typedef enum {
    DEFAULT_PATTERN
} PatternType;

typedef struct Weapon {
    int fire_rate;
    int cooldown;
    int attack_damage;
    PatternType pattern;
} Weapon;

DEFINE_COMPONENT_MANAGER(Player, 1);
DEFINE_COMPONENT_MANAGER(Weapon, 1);

DECLARE_SETTER_GETTER(Player, int, nbBombs);
DECLARE_SETTER_GETTER(Player, int, bombs);
DECLARE_SETTER_GETTER(Player, float, speed);
DECLARE_SETTER_GETTER(Player, float, focusSpeed);
DECLARE_SETTER_GETTER(Player, Entity, hitboxSpriteId);
DECLARE_SETTER_GETTER(Player, Entity, GrazeSpriteId);
DECLARE_SETTER_GETTER(Player, int, grazeRadius);

DECLARE_SETTER_GETTER(Weapon, int , fire_rate)
DECLARE_SETTER_GETTER(Weapon, int , cooldown)
DECLARE_SETTER_GETTER(Weapon, int , attack_damage)
DECLARE_SETTER_GETTER(Weapon, PatternType , pattern)

extern void Player_start(Pool *p, PlayerName name, PatternType type);
extern void Player_update(GameContext *ctx);
extern Entity Player_get_playerID(Pool *p);
extern float Player_GetX(Pool *p);
extern float Player_GetY(Pool *p);

extern void teleport_to_player_spawn(Pool *p, Entity e);
extern bool Damage_player(Pool *p, Entity player);