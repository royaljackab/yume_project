/**
 * @file straight_laser.h
 * @brief Fonctions permettant de créer et gêrer des lasers droits
 */

#pragma once

#include "systems/timer.h"
#include "component.h"
#include "assets.h"
#include "components/collision_rectangle.h"
#include "components/common.h"
#include "components/sprite.h"

#define LASER_STATE_WARNING -1
#define LASER_STATE_GROWING 0
#define LASER_STATE_DURATION 1 
#define LASER_STATE_DONE 2

typedef struct Pool Pool;

/**
 * @brief composante laser droit dans la pool
 * 
 */
typedef struct {
    float laserLength;
    float laserWidth;                 // taille actuelle affichée
    float laserMaxWidth;              // taille quand le laser s'élargit au max
    float intersectionWidth;          // largeur hitbox
    
    Timer timer;

} Straight_laser;

DEFINE_COMPONENT_MANAGER(Straight_laser, MAX_ENTITIES)
DECLARE_SETTER_GETTER(Straight_laser, float, laserLength)
DECLARE_SETTER_GETTER(Straight_laser, float, laserWidth)
DECLARE_SETTER_GETTER(Straight_laser, float, laserMaxWidth)
DECLARE_SETTER_GETTER(Straight_laser, float, intersectionWidth)
/**
 * @brief Met à jour un laser droit pour une frame.
 *
 * Le laser évolue selon son timer interne. Il peut passer à l'état suivant si son heure
 * d'activation ou de destruction est venue.
 *
 * @param p pool courante (toutes les données de l'ECS)
 * @param laserID id du laser à mettre à jour
 * @return true si le laser doit continuer d'exister, false s'il doit être supprimé.
 */
bool straight_laser_update(Pool *p, Entity laserID);

/**
 * @brief Met à jour tous les lasers droits présents dans la pool.
 *
 * Parcourt le Straight_laserManager et appelle straight_laser_update pour
 * chaque instance et marque les entités à supprimer si nécessaire.
 *
 * @param pool pool courante (toutes les données de l'ECS)
 */
void straight_lasers_update_all(Pool *pool);

/**
 * @brief Dessine un laser droit.
 *
 * Le dessin utilise la longueur et la largeur du composant ainsi que la
 * Position et le sprite fournis.
 *
 * @param laser laser à afficher
 * @param pos position du laser
 * @param sprite sprite utilisé pour le rendu du laser
 */
void straight_laser_draw(Straight_laser *laser, Position * pos, Sprite * sprite);

/**
 * @brief Dessine tous les lasers droits actifs du manager.
 *
 * @param laserManager Straight_laserManager
 * @param positionManager PositionManager
 * @param spriteManager SpriteManager
 */
void straight_lasers_draw_all(Straight_laserManager *laserManager, PositionManager * positionManager, SpriteManager * spriteManager);

/**
 * @brief Crée un laser droit et l'attache à une nouvelle entité.
 *
 * Paramètres temporels : warning, growing et duration sont des durées
 * en frames utilisées pour initialiser le timer du
 * composant.
 *
 * @param pool Pointeur vers la pool.
 * @param x Position X initiale du laser.
 * @param y Position Y initiale du laser.
 * @param angle Angle initial
 * @param length Longueur du laser.
 * @param maxWidth Largeur maximale du laser.
 * @param warning Durée avant la phase de croissance.
 * @param growing Durée de croissance pendant laquelle la largeur augmente.
 * @param duration Durée pendant laquelle la largeur reste maximale.
 * @param graphic ID du visuel à utiliser pour ce laser.
 * @return L'identifiant de l'entité créée contenant le Straight_laser.
 */
Entity straight_laser_create(Pool *pool, int x, int y, int angle, int length, int maxWidth, int warning, int growing, int duration, SpriteID graphic);

/**
 * @brief Variante de création marquant le laser comme projectile ennemi.
 *
 * Appelle straight_laser_create puis ajoute le flag FLAG_PROJECTILE_ENEMY
 * à la flagList de l'entité.
 *
 * @see straight_laser_create
 */
Entity straight_laser_enemy_create(Pool *pool, int x, int y, int angle, int length, int maxWidth, int warning, int growing, int duration, SpriteID graphic);