#pragma once

#include "ecs.h"

#include <stdint.h>
#include <string.h>

#define DEFINE_COMPONENT_MANAGER(Type, Number)                                 \
  typedef struct {                                                             \
    Type dense[Number];                                                        \
    Entity entity_lookup[Number];                                              \
    uint32_t sparse[MAX_ENTITIES];                                             \
    uint32_t count;                                                            \
  } Type##Manager;                                                             \
                                                                               \
  static inline void Type##_init(Type##Manager *mgr) {                         \
    mgr->count = 0;                                                            \
    memset(mgr->sparse, NULL_INDEX, sizeof(mgr->sparse));                      \
  }                                                                            \
                                                                               \
  static inline void Type##_add(Type##Manager *mgr, Entity e, Type data) {     \
    if (mgr->sparse[e] != NULL_INDEX) {                                        \
      mgr->dense[mgr->sparse[e]] = data;                                       \
      return;                                                                  \
    }                                                                          \
    uint32_t id = mgr->count++;                                                \
    if (id >= Number)                                                          \
      return;                                                                  \
    mgr->dense[id] = data;                                                     \
    mgr->sparse[e] = id;                                                       \
    mgr->entity_lookup[id] = e;                                                \
  }                                                                            \
                                                                               \
  static inline Type *Type##_get(Type##Manager *mgr, Entity e) {               \
    uint32_t id = mgr->sparse[e];                                              \
    if (id == NULL_INDEX)                                                      \
      return NULL;                                                             \
    return &mgr->dense[id];                                                    \
  }                                                                            \
                                                                               \
  static inline void Type##_remove(Type##Manager *mgr, Entity e) {             \
    uint32_t id = mgr->sparse[e];                                              \
    if (id == NULL_INDEX)                                                      \
      return;                                                                  \
    uint32_t last_id = --mgr->count;                                           \
    Entity last_e = mgr->entity_lookup[last_id];                               \
    mgr->dense[id] = mgr->dense[last_id];                                      \
    mgr->entity_lookup[id] = last_e;                                           \
    mgr->sparse[last_e] = id;                                                  \
    mgr->sparse[e] = NULL_INDEX;                                               \
  }

#define DECLARE_GETTER(Component, type, champ)                                 \
  static inline type Component##_get_##champ(Component *p) { return p->champ; }

#define DECLARE_SETTER(Component, type, champ)                                 \
  static inline void Component##_get_##champ(Component *p, type champ) {       \
    p->champ = champ;                                                          \
  }

#define DECLARE_SETTER_GETTER(Component, type, champ)                          \
  DECLARE_SETTER(Component, type, champ)                                       \
  DECLARE_GETTER(Component, type, champ)
