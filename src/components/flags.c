#include "components/flags.h"
#include "pool.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Affiche le type de flag dans la console (debug)
 * @param flag Pointeur vers le composant Flag
 */
void Flag_display(FlagType flag) {
    if (!flag) {
        printf("Flag: NULL\n");
        return;
    }

    const char *type_str;
    switch (flag) {
        case FLAG_NONE:           type_str = "NONE"; break;
        case FLAG_PLAYER:         type_str = "PLAYER"; break;
        case FLAG_ENEMY:          type_str = "ENEMY"; break;
        case FLAG_BOSS:           type_str = "BOSS"; break;
        case FLAG_PROJECTILE_PLAYER:  type_str = "PROJECTILE_PLAYER"; break;
        case FLAG_PROJECTILE_ENEMY:   type_str = "PROJECTILE_ENEMY"; break;
        case FLAG_POWERUP:        type_str = "POWERUP"; break;
        case FLAG_WALL:           type_str = "WALL"; break;
        case FLAG_INVINCIBLE:     type_str = "INVINCIBLE"; break;
        case FLAG_NO_DAMAGE_PLAYER:   type_str = "NO_DAMAGE_PLAYER"; break;
        default:                  type_str = "UNKNOWN"; break;
    }
    printf("Flag: [Type: %s]\n", type_str);
}


bool Flag_in_list(FlagType type, flagList *list) {
    if (!list || !list->flags) return false;
    for (int i = 0; i < list->size; i++) {
        if (list->flags[i] == type) return true;
    }
    return false;
}

bool Entity_has_flag_in_list(Pool *p, Entity entity, flagList *list) {
    if (!list) return false;
    flagList *entityFlags = flagList_get(&p->flagList, entity);
    if (!entityFlags || !entityFlags->flags) return false;
    for (int i = 0; i < entityFlags->size; i++) {
        if (Flag_in_list(entityFlags->flags[i], list)) return true;
    }
    return false;
}

bool Entity_has_flag(Pool *p, Entity entity, FlagType flag) {
    flagList *entityFlags = flagList_get(&p->flagList, entity);
    if (!entityFlags) return false;
    for (int i = 0; i < entityFlags->size; i++) {
        if (entityFlags->flags[i] == flag) return true;
    }
    return false;
}

bool flagList_add_element(flagList *list, FlagType type){
/** 
 * @brief ajoute un flag a la fin de la liste et met a jour size
 */
    if (list->size >= MAX_FLAGS) {
        printf("Erreur d'ajout de flag: flagList full.\n");
        return false;
    }
    list->flags[list->size] = type;
    list->size++;
    return true;
}

bool flagList_remove_element(flagList *list, FlagType type){
/**
 *trouve le type passé en paramètre, et supprime sa première occurrence de la liste en mettant a jour size
 */
    int index = -1;
    //trouve l'index du type a supprimer
    for (int i = 0; i < list->size; i++) {
        if (list->flags[i] == type) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Erreur de suppression de flag: type non trouvé.\n");
        return false;
    }
    // Décaler les éléments suivants pour combler le trou
    for (int i = index; i < list->size - 1; i++) {
        list->flags[i] = list->flags[i + 1];
    }
    list->size--;
    return true;
}

void flagList_destroy(Pool *pool, Entity id){
    /***
     * Détruit une flaglist dans la pool (y compris on libérant tout ce qui le compose, contrairement à flagList_remove)
     */

    flagList * list = flagList_get(&pool->flagList, id);

    if(!list){
        return;
    }
    if (list->flags) 
        free(list->flags);

    flagList_remove(&pool->flagList, id);
}

Entity flagList_attach_first_flag(Pool *p, Entity e, FlagType flag){
/**
 * Crée et attache une flagList avec un flag à une entité, attention overwrite toute flagList existante. Fait le malloc ici
 */
  FlagType * flagTypeList = malloc(sizeof(FlagType) * MAX_FLAGS);
  
  flagList flagList = {.flags = flagTypeList, .size = 0};
  flagList_add_element(&flagList, flag);
  flagList_add(&p->flagList, e, flagList);
  return e;
}

