#pragma once

#include <raylib.h>
#include <raymath.h>
#include <stdint.h>

#define NULL_INDEX 0xFFFFFFFF

#define MAX_ENTITIES 50000

// Une entité est un indice
typedef uint32_t Entity;

typedef struct BoxedEntity {
    Entity id;
    uint32_t unique_id;
} BoxedEntity;


