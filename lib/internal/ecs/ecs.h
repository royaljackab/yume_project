/**
 * @file ecs.h
 * @brief Types et déclarations utilisés dans l'ECS
 */

#pragma once

#include "raylib.h"
#include <raymath.h>
#include <stdint.h>

#define NULL_INDEX 0xFFFFFFFF

#define MAX_ENTITIES 100000

/**
 * @brief Une entité est un indice
 * 
 */
typedef uint32_t Entity;

/**
 * @brief Référence unique pour identifier une entité même après qu'elle ai quitté l'ECS
 * 
 */
typedef struct BoxedEntity {
    Entity id;
    uint32_t unique_id;
} BoxedEntity;


