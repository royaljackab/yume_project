/**
 * @file collision_entity.h
 * @brief fonctions de détection des collisions des entités
 */

#pragma once
#include "ecs/pool.h"
#include "player.h"
#include <raylib.h>

#define MAX_COLLISIONS 100000


/**
 * @brief Vérifie si une entité est touchée par au moins une autre entitée
 * @details Vérifie si un cercle ou rectangle de collision ayant les flags de la flagList passée en paramètre touche une entité spécifiée
 * 
 * @param p pool courante (toutes les données de l'ECS)
 * @param entity l'entité dont on cherche à savoir si elle est en collision
 * @param flags les entités en collision avec l'entitée que l'on étudie doivent disposer de ces flags, sinon elles sont ignorées
 * @return true si l'entité est touchée avec les flags spécifiés, false sinon
 */
extern bool Entity_is_hit(Pool *p, Entity entity, flagList * flags);

/**
 * @brief Vérifie si une entité est touchée par d'autres entitées et les liste dans un tableau
 * @details Vérifie si des cercles ou rectangles de collision ayant les flags de la flagList passée en paramètre touche une entité spécifiée, puis les ajoute à un tableau.
 * 
 * @param p pool courante (toutes les données de l'ECS)
 * @param entity l'entité dont on cherche à savoir si elle est en collision
 * @param flags les entités en collision avec l'entité que l'on étudie doivent disposer de ces flags, sinon elles sont ignorées
 * @param foundCollisions Un tableau d'ID, vide au départ, qui sera rempli avec toutes les entitées en collisions avec l'entité étudiée et remplissant le critère des flags
 * @param nbCollisions Un pointeur qui prendra la valeur du nombre de collisions valides trouvées
 * @return true si l'entité est touchée avec les flags spécifiés, false sinon
 */
extern bool Entity_find_hitters(Pool *p, Entity entity, flagList * flags, Entity foundCollisions[], int *nbCollisions);

/**
 * @brief Verifie si l'entité est touchée par un projectile ennemi, et lui inflige des dégâts
 * @note les projectiles ennemis sont identifiés par le flag FLAG_PROJECTILE_ENEMY
 * 
 * @param p pool courante (toutes les données de l'ECS)
 * @param entity l'entité dont on cherche à savoir si on doit lui infliger des dégâts
 * @return true si l'entité a subit des dégâts, false sinon
 */
extern bool Damage_entity_by_enemy_projectile(Pool *p, Entity entity);


/**
 * @brief Verifie si l'entité (utilisé surtout pour le joueur) est touchée par un projectile ennemi, et lui inflige des dégâts
 * 
 * @param ctx GameContext courant (toutes les données du jeu)
 * @param player L'ID de l'entité testé (le joueur)
 * @return true si le joueur a subit des dégats, faux sinon
 * 
 * @todo changer ctx par pool, pas besoin de tout le contexte
 * @todo changer pour trouver le joueur automatiquement
 */
extern bool Damage_player_by_enemy_projectile(GameContext *ctx, Entity player);

/**
 * @brief Crée automatiquement une hitbox circulaire d'une taille basée sur le sprite d'une entité
 * 
 * @param p pool courante (toutes les données de l'ECS)
 * @param entity l'entité cible
 * @return true, si l'ajout a marché, faux sinon
 */
extern bool collision_circle_add_scaled_with_sprite(Pool *p, Entity entity);

/**
 * @brief Vérifie si un cercle et un rectangle sont en collision.
 * 
 * @param cPos coordonnées 2D du cercle
 * @param radius rayon du cercle
 * @param rPos coordonnées 2D du coin du rectangle
 * @param w largeur du rectangle
 * @param h longueur du rectangle
 * @param angle angle de rotation du rectangle
 * @return true si les deux figures sont en collision, false sinon
 */
extern bool CheckCircleRotatedRect(Vector2 cPos, float radius, Vector2 rPos, float w, float h, float angle);

static bool Entity_is_hit_by_rectangle(Pool *p, Entity entity, flagList * laserFlags, Entity foundCollisions[], int * nbcollisions);
static bool Entity_is_hit_by_circle(Pool *p, Entity entity, flagList * BulletFlags, Entity foundCollisions[], int * nbcollisions);