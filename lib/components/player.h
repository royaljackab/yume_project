#pragma once

#include "component.h"
#include "collision_circle.h"
#include "common.h"
#include "systems/score.h"

/**
 * @file player.h
 * @brief Gestion du joueur : structures Player, Weapon
 */

#define INITIAL_PLAYER_LIVES 3

typedef struct GameContext GameContext;
typedef struct Pool Pool;

/**
 * @brief Identifiants de personnages jouables.
 */
typedef enum {
    TEST_PLAYER
} PlayerName;

/**
 * @brief Composante Player dans l'ECS
 * Représente l'état et les attributs d'un joueur. Le joueur est la seule entité a utiliser cette composante.
 */
typedef struct Player {

    int nbBombs; // Nombre de bombs maximal
    int bombs;  //nombre de bombe actuel

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

/**
 * @brief Composante weapon dans l'ECS
 * Contient les informations d'une arme du joueur. Le joueur est la seule entité a utiliser cette composante.
 */
typedef struct Weapon {
    int fire_rate; /**< Cadence de tir (frames entre tirs) */
    int cooldown; /**< Compteur interne de cooldown */
    int attack_damage; /**< Dégâts infligés par tir */
    PatternType pattern; /**< Pattern d'attaque utilisé */
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

/**
 * @brief Initialise et crée l'entité joueur dans la pool.
 *
 * Crée l'entité joueur, initialise ses composants: Player, Weapon,
 * Position, Sprite, Collision_circle, Life, etc.) et place le joueur
 * au point de spawn.
 *
 * @param p pool courante (toutes les données de l'ECS).
 * @param name Type de joueur à instancier.
 * @param type Type de pattern d'arme à utiliser.
 */
extern void Player_start(Pool *p, PlayerName name, PatternType type);

/**
 * @brief Mise à jour par frame du joueur (input, tir, focus, graze, dégâts...).
 *
 * Appelée depuis la boucle principale pour traiter l'input et les actions du
 * joueur.
 *
 * @param ctx GameContext courant (toutes les données du jeu)
 */
extern void Player_update(GameContext *ctx);

/**
 * @brief Récupère l'id du joueur.
 * Il n'y a toujours qu'un seul joueur actif, on peut donc l'identifier.
 *
 * @param p pool courante (toutes les données de l'ECS)
 * @return id du joueur (index 0 du PlayerManager).
 */
extern Entity Player_get_playerID(Pool *p);

/**
 * @brief Récupère la position X du joueur.
 *
 * @param p pool courante (toutes les données de l'ECS)
 * @return coordonée X du joueur
 */
extern float Player_GetX(Pool *p);

/**
 * @brief Récupère la position Y du joueur.
 *
 * @param p pool courante (toutes les données de l'ECS)
 * @return coordonée Y du joueur
 */
extern float Player_GetY(Pool *p);

/**
 * @brief Téléporte une entité vers le point de spawn du joueur.
 *
 * Utilisé pour repositionner le joueur après qu'il ai subit des dégâts où au début du jeu.
 *
 * @param p pool courante (toutes les données de l'ECS)
 * @param e Entité à téléporter.
 */
extern void teleport_to_player_spawn(Pool *p, Entity e);

/**
 * @brief Inflige des dégâts au joueur et gère les conséquences (respawn,
 * invincibilité, effets sonores... )
 *
 * @param ctx GameContext courant (toutes les données du jeu)
 * @param player joueur à blesser
 * @return true si l'opération a été effectuée sans erreur, false sinon.
 */
extern bool Damage_player(GameContext *ctx, Entity player);
extern void Player_bomb(GameContext *ctx, Entity player);