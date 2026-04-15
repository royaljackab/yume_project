#pragma once

#include "common.h"
#include "ecs.h"
#include "flags.h"
#include <stdbool.h>


/** @brief Supprime un objet du pool
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet à supprimer
 */
void obj_Delete(Pool *p, Entity objId);

/** @brief Définit la visibilité d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param visible La visibilité de l'objet
 */
void obj_SetVisible(Pool *p, Entity objId, bool visible);

/** @brief Vérifie si un objet est visible
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return true si l'objet est visible, false sinon
 */
bool obj_IsVisible(Pool *p, Entity objId);

/** @brief Définit la priorité de rendu d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param renderPriority La priorité de rendu de l'objet
 */
void obj_SetRenderPriority(Pool *p, Entity objId, int renderPriority);

/** @brief Récupère la priorité de rendu d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return La priorité de rendu de l'objet
 */
int obj_GetRenderPriority(Pool *p, Entity objId);

/** @brief Récupère le type d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return Le type de l'objet
 */
EntityType obj_GetType(Pool *p, Entity objId);

/*----------------------------------------------------*/

/** @brief Définit la position X d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param x La position X de l'objet
 */
void obj_SetX(Pool *p, Entity objId, float x);

/** @brief Définit la position Y d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param y La position Y de l'objet
 */
void obj_SetY(Pool *p, Entity objId, float y);

/** @brief Définit la position d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param x La position X de l'objet
 * @param y La position Y de l'objet
 */
void obj_SetPosition(Pool *p, Entity objId, float x, float y);

/** @brief Définit l'angle d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param angle L'angle de l'objet
 */
void obj_SetAngle(Pool *p, Entity objId, float angle);

/** @brief Définit la taille d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param scalex La taille X de l'objet
 * @param scaley La taille Y de l'objet
 */
void obj_SetScaleX(Pool *p, Entity objId, float scalex);

/** @brief Définit la taille Y d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param scaley La taille Y de l'objet
 */
void obj_SetScaleY(Pool *p, Entity objId, float scaley);

/** @brief Définit la taille d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param scalex La taille X de l'objet
 * @param scaley La taille Y de l'objet
 */
void obj_SetScale(Pool *p, Entity objId, float scalex, float scaley);

/** @brief Définit la couleur d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param r La composante rouge de la couleur de l'objet (0-255)
 * @param g La composante verte de la couleur de l'objet (0-255)
 * @param b La composante bleue de la couleur de l'objet (0-255)
 */
void obj_SetColor(Pool *p, Entity objId, float r, float g, float b);

/** @brief Définit la transparence d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param alpha La transparence de l'objet (0-255)
 */
void obj_SetAlpha(Pool *p, Entity objId, float alpha);

/** @brief Récupère la position X d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return La position X de l'objet
 */
float obj_GetX(Pool *p, Entity objId);

/** @brief Récupère la position Y d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return La position Y de l'objet
 */
float obj_GetY(Pool *p, Entity objId);

/** @brief Récupère la position d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return La position de l'objet
 */
Vector2 obj_GetPosition(Pool *p, Entity objId);

/** @brief Récupère l'angle d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return L'angle de l'objet
 */
float obj_GetAngle(Pool *p, Entity objId);

/** @brief Récupère la taille X d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return La taille X de l'objet
 */
float obj_GetScaleX(Pool *p, Entity objId);

/** @brief Récupère la taille Y d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return La taille Y de l'objet
 */
float obj_GetScaleY(Pool *p, Entity objId);

/** @brief Récupère la taille d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return La taille de l'objet
 */
Vector2 obj_GetScale(Pool *p, Entity objId);

/** @brief Récupère la couleur d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return La couleur de l'objet
 */
Color obj_GetColor(Pool *p, Entity objId);

/** @brief Récupère la transparence d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return La transparence de l'objet
 */
float obj_GetAlpha(Pool *p, Entity objId);



/*----------------------------------------------------*/

/** @brief Définit l'accélération d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param accel L'accélération de l'objet
 */
void obj_SetAcceleration(Pool *p, Entity objId, float accel);

/** @brief Définit la vitesse d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param speed La vitesse de l'objet
 */
void obj_SetSpeed(Pool *p, Entity objId, float speed);

/** @brief Définit la vitesse angulaire d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param angvel La vitesse angulaire de l'objet
 */
void obj_SetAngularSpeed(Pool *p, Entity objId, float angvel);

/** @brief Définit la force X d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param forcex La force X de l'objet
 */
void obj_SetForceX(Pool *p, Entity objId, float forcex);

/** @brief Définit la force Y d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param forcey La force Y de l'objet
 */
void obj_SetForceY(Pool *p, Entity objId, float forcey);

/** @brief Définit la force d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param forcex La force X de l'objet
 * @param forcey La force Y de l'objet
 */
void obj_SetForce(Pool *p, Entity objId, float forcex, float forcey);

/** @brief Définit la vitesse de déplacement d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param velX La vitesse X de l'objet
 * @param velY La vitesse Y de l'objet
 */
void obj_SetVelocity(Pool *p, Entity objId, float velX, float velY);

/** @brief Définit la vitesse maximale d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param maxspd La vitesse maximale de l'objet
 */
void obj_SetMaxSpd(Pool *p, Entity objId, float maxspd);

/** @brief Définit la vitesse minimale d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param minspd La vitesse minimale de l'objet
 */
void obj_SetMinSpd(Pool *p, Entity objId, float minspd);

/** @brief Supprime la vitesse maximale d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 */
void obj_UnsetMaxSpd(Pool *p, Entity objId);

/** @brief Supprime la vitesse minimale d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 */
void obj_UnsetMinSpd(Pool *p, Entity objId);

/** @brief Récupère la vitesse d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return La vitesse de l'objet
 */
float obj_GetSpeed(Pool *p, Entity objId);

/** @brief Récupère la vitesse angulaire d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return La vitesse angulaire de l'objet
 */
float obj_GetAngularSpeed(Pool *p, Entity objId);

/*----------------------------------------------------*/

/** @brief Récupère la vie d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return La vie de l'objet
 */
int obj_GetLife(Pool *p, Entity objId);

/** @brief Récupère la vie maximale d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return La vie maximale de l'objet
 */
int obj_GetMaxLife(Pool *p, Entity objId);

/** @brief Vérifie si un objet est mort
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return true si l'objet est mort, false sinon
 */
bool obj_IsDead(Pool *p, Entity objId);


/** @brief Définit la vie d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param life La vie de l'objet
 */
void obj_SetLife(Pool *p, Entity objId, int life);

/** @brief Définit la vie maximale d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param maxLife La vie maximale de l'objet
 */
void obj_SetMaxLife(Pool *p, Entity objId, int maxLife);

/*----------------------------------------------------*/

/** @brief Récupère le tag d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return Le tag de l'objet
 */
Tag obj_GetTag(Pool *p, Entity objId);

/** @brief Définit le tag d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param tag Le tag de l'objet
 */
void obj_SetTag(Pool *p, Entity objId, Tag tag);

/** @brief Définit le rayon de la hitbox d'un objet
 * @param p Le pool contenant les objets
 * @param objID L'ID de l'objet
 * @param radius Le rayon de la hitbox de l'objet
 */
void obj_SetHitboxRadius(Pool *p, Entity objID, float radius);

/*----------------------------------------------------*/

/** @brief Ajoute un drapeau à un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param flag Le drapeau à ajouter
 */
void obj_AddFlag(Pool *p, Entity objId, FlagType flag);

/** @brief Supprime un drapeau d'un objet
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param flag Le drapeau à supprimer
 */
void obj_RemoveFlag(Pool *p, Entity objId, FlagType flag);



/*----------------------------------------------------*/




/** @brief Définit la vitesse d'un objet "loose head"
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param speed La vitesse à définir
 */
void obj_looseHead_SetSpeed(Pool *p, Entity objId, float speed);

/** @brief Définit la vitesse maximale d'un objet "loose head"
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param speed La vitesse maximale à définir
 */
void obj_looseHead_SetMaxSpd(Pool *p, Entity objId, float speed);

/** @brief Définit la vitesse minimale d'un objet "loose head"
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param speed La vitesse minimale à définir
 */
void obj_looseHead_SetMinSpd(Pool *p, Entity objId, float speed);

/** @brief Supprime la vitesse maximale d'un objet "loose head"
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 */
void obj_looseHead_UnsetMaxSpd(Pool *p, Entity objId);

/** @brief Supprime la vitesse minimale d'un objet "loose head"
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 */
void obj_looseHead_UnsetMinSpd(Pool *p, Entity objId);

/** @brief Définit l'accélération d'un objet "loose head"
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param accel L'accélération à définir
 */
void obj_looseHead_SetAcceleration(Pool *p, Entity objId, float accel);

/** @brief Définit la vitesse angulaire d'un objet "loose head"
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param angularSpeed La vitesse angulaire à définir
 */
void obj_looseHead_SetAngularSpeed(Pool *p, Entity objId, float angularSpeed);

/** @brief Définit la vitesse d'un objet "loose head"
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param velocity La vitesse à définir
 */
void obj_looseHead_SetVelocity(Pool *p, Entity objId, Vector2 velocity);

/** @brief Définit la force horizontale d'un objet "loose head"
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param forceX La force horizontale à définir
 */
void obj_looseHead_SetForceX(Pool *p, Entity objId, float forceX);

/** @brief Définit la force verticale d'un objet "loose head"
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param forceY La force verticale à définir
 */
void obj_looseHead_SetForceY(Pool *p, Entity objId, float forceY);

/** @brief Définit la force d'un objet "loose head"
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @param force La force à définir
 */
void obj_looseHead_SetForce(Pool *p, Entity objId, Vector2 force);

/** @brief Récupère l'ID de l'objet "loose head"
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return L'ID de l'objet "loose head"
 */
Entity obj_GetlooseHeadId(Pool * p, Entity objId);

/** @brief Récupère la vitesse d'un objet "loose head"
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return La vitesse de l'objet "loose head"
 */
float obj_looseHead_GetSpeed(Pool *p, Entity objId);

/** @brief Récupère la vitesse maximale d'un objet "loose head"
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return La vitesse maximale de l'objet "loose head"
 */
float obj_looseHead_GetMaxSpd(Pool *p, Entity objId);

/** @brief Récupère la vitesse minimale d'un objet "loose head"
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return La vitesse minimale de l'objet "loose head"
 */
float obj_looseHead_GetMinSpd(Pool *p, Entity objId);

/** @brief Récupère l'accélération d'un objet "loose head"
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return L'accélération de l'objet "loose head"
 */
float obj_looseHead_GetAcceleration(Pool *p, Entity objId);

/** @brief Récupère la vitesse angulaire d'un objet "loose head"
 * @param p Le pool contenant les objets
 * @param objId L'ID de l'objet
 * @return La vitesse angulaire de l'objet "loose head"
 */
float obj_looseHead_GetAngularSpeed(Pool *p, Entity objId);
