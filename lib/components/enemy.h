/**
 * @file enemy.h
 * @brief Composante des ennemis
 *
 * Gère la création et le comportement des ennemis.
 * Chaque ennemi a un pattern de mouvement et peut tirer des bullets.
 *
 * DONNÉES GÉRÉES :
 *  - enemyType    : Le type d'ennemi (pour adapter le comportement)
 *  - shootTimer   : Compteur de frames avant le prochain tir
 *  - shootRate    : Nombre de frames entre chaque tir
 *  - canShoot     : Si l'ennemi peut tirer ou non
 */

#pragma once

#include "ecs/component.h"
#include "components/common.h"
#include <stdbool.h>

typedef struct Pool Pool;

/* Types d'ennemis */
typedef enum {
    ENEMY_FAIRY_BLUE,    // Petite fée bleue basique
    ENEMY_FAIRY_RED,     // Petite fée rouge (futur)
} EnemyType;

/* Patterns de mouvement */
typedef enum {
    PATTERN_STRAIGHT_DOWN,  // Descend tout droit
    PATTERN_ZIGZAG,         // Descend en zigzag
    PATTERN_CIRCLE_IN,      // Entre en arc de cercle
} MovementPattern;

/* Composant Enemy */
typedef struct Enemy {
    EnemyType enemyType;
    MovementPattern pattern;

    int shootTimer;       // Compteur courant
    int shootRate;        // Frames entre chaque tir (0 :: ne tire pas)
    bool canShoot;

    float patternTimer;   // Timer pour le pattern de mouvement
    float patternParam;   // Paramètre du pattern (amplitude zigzag, rayon cercle....;)
} Enemy;

DEFINE_COMPONENT_MANAGER(Enemy, MAX_ENTITIES)

DECLARE_SETTER_GETTER(Enemy, EnemyType, enemyType)
DECLARE_SETTER_GETTER(Enemy, MovementPattern, pattern)
DECLARE_SETTER_GETTER(Enemy, int, shootTimer)
DECLARE_SETTER_GETTER(Enemy, int, shootRate)
DECLARE_SETTER_GETTER(Enemy, bool, canShoot)
DECLARE_SETTER_GETTER(Enemy, float, patternTimer)
DECLARE_SETTER_GETTER(Enemy, float, patternParam)

/**
 * @brief Crée un ennemi basique qui descend et tire
 * @param p Pool courante
 * @param x Position x initiale
 * @param y Position y initiale
 * @param speed Vitesse de déplacement
 * @param type Type d'ennemi
 * @param pattern Pattern de mouvement
 * @param hp Points de vie
 * @return L'entité créée
 */
extern Entity Enemy_spawn(Pool *p, float x, float y, float speed, 
                          EnemyType type, MovementPattern pattern, int hp);

/**
 * @brief Met à jour tous les ennemis (mouvement + tir)
 * @param p Pool courante
 */
extern void Enemy_update_all(Pool *p);