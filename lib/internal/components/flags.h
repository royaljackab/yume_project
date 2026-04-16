/**
 * @file flags.h
 * @brief Les flags sont des critères qu'une entité peut avoir ou ne pas avoir. 
 * Chaque flag active un comportement particulier dans le jeu.
 */

#pragma once

#define MAX_FLAGS 30
#include "ecs/component.h"
typedef struct Pool Pool;


/**
 * @brief Identificateurs uniques de certaines propriétés
 * 
 */
typedef enum {
    FLAG_NONE = 0,
    FLAG_PLAYER,
    FLAG_ENEMY,
    FLAG_BOSS,
    FLAG_BULLET_PLAYER, //identifie une balle (pas un laser) créée par le joueur
    FLAG_PROJECTILE_PLAYER, //identifie un projectile (quelconque) créé par le joueur
    FLAG_PROJECTILE_ENEMY, //identifie un projectile créé par un ennemi
    FLAG_NO_DAMAGE_PLAYER, //n'inflige pas de dégat au joueur
    FLAG_POWERUP,
    FLAG_WALL,
    FLAG_PLAYER_HIT,
    FLAG_INVINCIBLE, 
    FLAG_DECREASE_SCORE, //fait perdre des points au joueur s'il détruit l'entité (ex: certains ennemis)
} FlagType;

/**
 * @brief composante flagList dans l'ECS
 * Une entité peut disposer de plusieurs flags, qui sont contenus dans une flaglist
 * 
 */
typedef struct {
    FlagType *flags;
    int size;
} flagList;



/* Génération automatique du FlagManager et des fonctions init/add/get/remove */
DEFINE_COMPONENT_MANAGER(flagList, MAX_ENTITIES)
DECLARE_GETTER(flagList, FlagType *, flags)
DECLARE_SETTER_GETTER(flagList, int, size)


/**
 * @brief Vérifie si un FlagType est présent dans une flagList.
 *
 * Parcourt le tableau de flags de la liste et retourne vrai si le type est trouvé.
 *
 * @param type Le FlagType recherché.
 * @param list Pointeur vers la flagList à inspecter.
 * @return true si le flag est présent, false sinon.
 */
bool Flag_in_list(FlagType type, flagList *list);

/**
 * @brief Ajoute un FlagType à la fin d'une flagList.
 *
 * Si la liste contient déjà `MAX_FLAGS` éléments, l'ajout échoue.
 *
 * @param list Pointeur vers la flagList cible.
 * @param type Le FlagType à ajouter.
 * @return true si l'ajout a réussi, false sinon.
 */
bool flagList_add_element(flagList *list, FlagType type);

/**
 * @brief Supprime la première occurrence d'un FlagType dans une flagList.
 *
 * Décale les éléments suivants pour combler le trou et décrémente `size`.
 *
 * @param list Pointeur vers la flagList.
 * @param type Le FlagType à supprimer.
 * @return true si un élément a été supprimé, false si le type n'a pas été trouvé.
 */
bool flagList_remove_element(flagList *list, FlagType type);

/**
 * @brief Vérifie si une entité possède au moins un flag présent dans une flagList donnée.
 *
 * Récupère la flagList de l'entité depuis la `Pool` et compare chaque flag avec
 * les éléments de la liste fournie.
 *
 * @param p pool courante (toutes les données de l'ECS)
 * @param entity Identifiant de l'entité à vérifier.
 * @param list Pointeur vers la flagList de référence.
 * @return true si l'entité possède au moins un flag contenu dans `list`, false sinon.
 */
bool Entity_has_flag_in_list(Pool *p, Entity entity, flagList *list);

/**
 * @brief Détruit la flagList attachée à une entité dans la Pool.
 *
 * Libère la mémoire allouée pour le tableau de flags puis retire le composant
 * de la `Pool`.
 *
 * @param p pool courante (toutes les données de l'ECS)
 * @param e Identifiant de l'entité dont la flagList doit être détruite.
 */
void flagList_destroy(Pool *p, Entity e);

/**
 * @brief Vérifie si une entité possède un flag précis.
 *
 * @param p pool courante (toutes les données de l'ECS)
 * @param entity Identifiant de l'entité à inspecter.
 * @param flag Le FlagType recherché.
 * @return true si l'entité possède `flag`, false sinon.
 */
bool Entity_has_flag(Pool *p, Entity entity, FlagType flag);

/**
 * @brief Crée et attache une nouvelle flagList initialisée avec un flag.
 *
 * Alloue un tableau de taille `MAX_FLAGS`, crée une flagList et l'attache à
 * l'entité. ATTENTION : écrase toute flagList existante pour cette entité.
 *
 * @param p pool courante (toutes les données de l'ECS)
 * @param e Identifiant de l'entité cible.
 * @param flag Le premier FlagType à ajouter à la liste.
 * @return L'entité passée en paramètre (utile pour chaînage).
 */
Entity flagList_attach_first_flag(Pool *p, Entity e, FlagType flag);

/**
 * @brief Affiche le type de flag dans la console (debug)
 * @param flag le flagType à afficher
 */
void Flag_display(FlagType flag);