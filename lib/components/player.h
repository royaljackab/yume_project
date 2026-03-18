#pragma once

#include "component.h"

typedef struct GameContext GameContext;
typedef struct Pool Pool;

typedef enum {
    TEST_PLAYER
} PlayerName;

typedef struct Player {
    int nbLives;
    int lives;

    int nbBombs;
    int bombs;

    float speed;
    float focusSpeed;

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

DECLARE_SETTER_GETTER(Player, int, nbLives);
DECLARE_SETTER_GETTER(Player, int, lives);
DECLARE_SETTER_GETTER(Player, int, nbBombs);
DECLARE_SETTER_GETTER(Player, int, bombs);
DECLARE_SETTER_GETTER(Player, float, speed);
DECLARE_SETTER_GETTER(Player, float, focusSpeed);

extern void Player_start(Pool *p, PlayerName name, PatternType type);
extern void Player_update(GameContext *ctx);



