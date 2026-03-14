#pragma once

#include "ecs.h"

#include <stdint.h>
#include <string.h>


/** 
 * Créé un composant dans l'ECS.
 * Params:
 * * * Type   : Le nom de la composante à créer
 * * * Number : Doit être égal à MAX_ENTITIES (sauf si la composante est très lourde)
 * 
 * Ce composant est une structure contenant:
 * - tableau "dense"          : contenant toutes les instances du composant défini
 * - tableau "sparse"         : renvoie l'ID d'un élément dans le tableau sparse à partir de l'ID d'un élément dans le tableau dense 
 *                              (en pratique on le l'utilise pas directement, privilégier get)
 * - tableau "entity_lookup"  : renvoie l'ID d'un élément dans le tableau dense à partir de l'ID d'un élément dans le tableau  
 *                              (c'est une Entity.)
 * - count                    : Le nombre d'éléments actuels du composant 
 * 
 * Ce composant dispose de 4 fonctions:
 * Type##_init(Type##Manager *mgr):
 * s'utilise dans la fonction d'initialisation d'un gamestate
 * Params:
 * * * mgr  : le manager de la composante
 * 
 * 
 * Type##_add(Type##Manager *mgr, Entity e, Type data):
 * append une nouvelle instance du composant à "dense".
 * Params:
 * * * mgr  : le manager de la composante
 * * * e    : l'ID de l'instance à ajouter
 * * * data : Une instance de la composante, elle est mise dans dense
 *            Ne doit PAS être déclaré dynamiquement
 * 
 * 
 * Type##_get(Type##Manager *mgr, Entity e):
 * return la valeur de la composante pour un ID
 * Params:
 * * * mgr  : le manager de la composante
 * * * e    : l'ID de l'instance dont on cherche la valeur de la composante Type
 * 
 * 
 * static inline void Type##_remove(Type##Manager *mgr, Entity e)
 * détruit une instance de la composante
 * Params:
 * * * mgr  : le manager de la composante
 * * * e    : l'ID de l'instance à détruire
 * 
*/

#define DEFINE_COMPONENT_MANAGER(Type, Number)                                 \
  typedef struct {                                                             \
    Type dense[Number];                                                        \
    Entity entity_lookup[Number];                                              \
    uint32_t sparse[MAX_ENTITIES];                                             \
    int count;                                                                 \
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
    int id = mgr->count++;                                                     \
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
    int last_id = --mgr->count;                                                \
    Entity last_e = mgr->entity_lookup[last_id];                               \
    mgr->dense[id] = mgr->dense[last_id];                                      \
    mgr->entity_lookup[id] = last_e;                                           \
    mgr->sparse[last_e] = id;                                                  \
    mgr->sparse[e] = NULL_INDEX;                                               \
  }

#define DECLARE_GETTER(Component, type, champ)                                 \
  static inline type Component##_get_##champ(Component *p) { return p->champ; }
// Sprite_get_textureID();

#define DECLARE_SETTER(Component, type, champ)                                 \
  static inline void Component##_set_##champ(Component *p, type champ) {       \
    p->champ = champ;                                                          \
  }
// Sprite_set_textureID(&textureID);

#define DECLARE_SETTER_GETTER(Component, type, champ)                          \
  DECLARE_SETTER(Component, type, champ)                                       \
  DECLARE_GETTER(Component, type, champ)
