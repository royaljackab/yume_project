#pragma once

#define MAX_LOOSE_NODES 1000
#define DURATION_ETERNAL 20000
#define MAX_LOOSE_LASER 282

#include "systems/timer.h"
#include "ecs/component.h"
#include <raymath.h>

#include "components/sprite.h"
#include "components/common.h"

typedef struct Pool Pool;
/**
 * @brief Représente un loose laser — un filament composé de plusieurs nodes.
 *
 * Le node à l'avant (head) se comporte comme une `Bullet` avec physique. Les
 * nodes suivants conservent l'historique des positions du head pour créer
 * un effet de traînée (tail).
 */
typedef struct {
    Entity *looseNodes; /**< Tableau d'entités représentant les nodes (head + historique) */
    int looseNodeCount; /**< Nombre actuel de nodes valides dans `looseNodes` */
    float looseTargetLength; /**< Longueur cible du laser; la chaîne est tronquée si dépassée */
    float looseWidth; /**< Épaisseur du laser en pixels */
    Timer looseTimer; /**< Timer gérant la durée de vie du laser */
    Color looseColor; /**< Couleur utilisée pour le rendu (affichage uniquement) */
} Loose_laser;

DEFINE_COMPONENT_MANAGER(Loose_laser, MAX_LOOSE_LASER)

DECLARE_GETTER(Loose_laser, Entity *, looseNodes)
DECLARE_SETTER_GETTER(Loose_laser, int, looseNodeCount)
DECLARE_SETTER_GETTER(Loose_laser, float, looseTargetLength)
DECLARE_SETTER_GETTER(Loose_laser, float, looseWidth)

/**
 * @brief Met à jour un loose laser pour une frame.
 *
 * - Ajoute un nouveau node si la capacité le permet.
 * - Décale les positions des nodes pour conserver l'historique.
 * - Tronque la chaîne si la longueur actuelle dépasse `looseTargetLength`.
 * - Incrémente le timer et indique si le laser doit persister.
 *
 * @param pool pool courante (toutes les données de l'ECS)
 * @param laser Pointeur vers le `Loose_laser` à mettre à jour.
 * @return true si le laser doit continuer d'exister; false s'il doit être supprimé.
 */
bool loose_laser_update(Pool *pool, Loose_laser *laser);

/**
 * @brief Met à jour tous les loose lasers de la `Pool`.
 *
 * Parcourt le manager de Loose_laser, appelle loose_laser_update pour
 * chaque instance et marque les entités à supprimer si nécessaire.
 *
 * @param pool pool courante (toutes les données de l'ECS)
 */
void loose_lasers_update_all(Pool *pool);

/**
 * @brief Crée et attache un `Loose_laser` à une nouvelle entité.
 *
 * - Alloue un tableau de `MAX_LOOSE_NODES` pour stocker les nodes.
 * - Crée la tête du laser en spawnant une `Bullet` ennemie.
 * - Initialise le timer, la longueur cible, la largeur et la couleur.
 * - Ajoute le composant `Loose_laser` à l'entité et assigne le tag `ENT_LOOSE_LASER`.
 *
 * @param pool pool courante (toutes les données de l'ECS)
 * @param x Position X initiale de la tête.
 * @param y Position Y initiale de la tête.
 * @param speed Vitesse initiale de la tête (transmise au spawn de la bullet).
 * @param length Longueur cible du laser.
 * @param width Épaisseur du laser.
 * @param duration Durée de vie (unités du timer utilisée par le système).
 * @param color Couleur du laser pour le rendu.
 * @return L'identifiant de l'entité créée contenant le Loose_laser.
 */
Entity loose_laser_create(Pool * pool, int x, int y, float speed, float length, float width, int duration, Color color);

/**
 * @brief Crée un node (entité) pour un loose laser.
 *
 * Ajoute une `Position` et une `Collision_circle` à l'entité nouvellement créée.
 *
 * @param pool pool courante (toutes les données de l'ECS)
 * @param x Position X initiale du node.
 * @param y Position Y initiale du node.
 * @param radius Rayon du cercle de collision.
 * @return Identifiant de l'entité node créée.
 */
Entity node_create(Pool * pool, float x, float y, float radius);

/**
 * @brief Détruit un `Loose_laser` et libère ses ressources.
 *
 * Tue toutes les entités correspondant aux nodes, libère le tableau de nodes
 * et retire le composant du manager.
 *
 * @param pool pool courante (toutes les données de l'ECS)
 * @param id Identifiant de l'entité contenant le `Loose_laser`.
 */
void Loose_laser_destroy(Pool * pool, Entity id);

/**
 * @brief Dessine un Loose_laser.
 *
 * Ne fait rien si moins de deux nodes sont présents. Parcourt les segments,
 * calcule les normales et dessine des triangles et des cercles pour lisser les
 * jonctions.
 *
 * @param laser Pointeur vers le Loose_laser à afficher.
 * @param positionManager Pointeur vers le PositionManager pour consulter
 * les positions des nodes.
 */
void draw_loose_laser(Loose_laser *laser, PositionManager *positionManager);

/**
 * @brief Dessine tous les loose lasers actifs du manager.
 *
 * @param looseManager Pointeur vers le Loose_laserManager.
 * @param positionManager Pointeur vers le PositionManager.
 */
void draw_all_loose_lasers(Loose_laserManager *looseManager, PositionManager *positionManager);
