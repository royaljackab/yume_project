/**
 * @file flags.h
 * @brief Composant Flag pour identifier le type d'une entité
 */

#pragma once

#define MAX_FLAGS 20
#include "ecs/component.h"
typedef struct Pool Pool;

/* Types de flags pour les entités */
typedef enum {
    FLAG_NONE = 0,
    FLAG_PLAYER,
    FLAG_ENEMY,
    FLAG_BOSS,
    FLAG_BULLET_PLAYER,
    FLAG_BULLET_ENEMY,
    FLAG_POWERUP,
    FLAG_WALL
} FlagType;

/* Liste de flags */
typedef struct {
    FlagType *flags;
    int size;
} flagList;



/* Génération automatique du FlagManager et des fonctions init/add/get/remove */
DEFINE_COMPONENT_MANAGER(flagList, MAX_ENTITIES)
DECLARE_GETTER(flagList, FlagType *, flags)
DECLARE_SETTER_GETTER(flagList, int, size)

bool Flag_in_list(FlagType type, flagList *list);
bool flagList_add_element(flagList *list, FlagType type);
bool Entity_has_flag_in_list(Pool *p, Entity entity, flagList *list);
void flagList_destroy(Pool *p, Entity e);