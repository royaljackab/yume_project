#ifndef ECS_H
#define ECS_H

#include <inttypes.h>
#include <stddef.h>

#define ENTITY_FLAG_ALIVE 1

typedef struct {
    uint32_t id;
} Entity;

typedef struct {
    uint32_t count;
    uint32_t cap;
    uint32_t *list;
} QueryResult;

#endif