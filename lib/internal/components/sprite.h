/**
 * @file sprite.h
 * @brief Composante du sprite
 *
 * Ce module gère l'affectation d'un sprite à une entité et son affichage.
 *
 * DONNÉES :
 *  - textureID : ID définies dans assets.h
 *  - srcRect : Rectangle définissant le sprite
 *  - center : Point du srcRect représentant son centre. (pour hitbox et la
 * rotation)
 *  - scale : Agrandissement de l'image
 *  - color : Couleur d'affichage raylib
 *  - renderPriority : Priorité d'affichage (1 - 100) -> z bas <=> affiché en
 * fond
 *  - display : activation de l'affichage
 */

#pragma once

#include "components/common.h"

#include "ecs/component.h"
#include <raylib.h>
#include <stdbool.h>

typedef struct Pool Pool;

/**
 * @brief Composante Sprite dans l'ECS
 * Cette composante sert à associer une texture à une Entité.
 * Contient les informations d'affichage de la texture ainsi que de son animation.
 * 
 */
typedef struct {
  int textureID;
  Rectangle srcRect;
  Vector2 center;
  Vector2 scale;
  Color color;
  int renderPriority; // Pour afficher devant/derriere d'autres..
  bool display;

  // Pour l'animation
  bool isAnimated;
  int animFrameCount;
  int animSpeed;
  int animTimer;
  int currentFrame;
  int frameWidth;
  float rotation;
  Vector2 animStart;

} Sprite;

DEFINE_COMPONENT_MANAGER(Sprite, MAX_ENTITIES)
DECLARE_SETTER_GETTER(Sprite, int, textureID)
DECLARE_SETTER_GETTER(Sprite, Rectangle, srcRect)
DECLARE_SETTER_GETTER(Sprite, Vector2, center)
DECLARE_SETTER_GETTER(Sprite, float, rotation)
DECLARE_SETTER_GETTER(Sprite, Vector2, scale)
DECLARE_SETTER_GETTER(Sprite, Color, color)
DECLARE_SETTER_GETTER(Sprite, int, renderPriority)
DECLARE_SETTER_GETTER(Sprite, int, display)
DECLARE_SETTER_GETTER(Sprite, bool, isAnimated)
DECLARE_SETTER_GETTER(Sprite, int, animFrameCount)
DECLARE_SETTER_GETTER(Sprite, int, animSpeed)
DECLARE_SETTER_GETTER(Sprite, int, animTimer)
DECLARE_SETTER_GETTER(Sprite, int, currentFrame)
DECLARE_SETTER_GETTER(Sprite, int, frameWidth)
DECLARE_SETTER_GETTER(Sprite, Vector2, animStart)

/**
 * @brief Ajoute une texture avec les valeurs pas défauts
 * @param sprite Composante du sprite
 * @param renderPriority Layer a laquelle on affiche le sprite
 * @param textureID ID de la texture a utiliser (assets.h)
 *
 * VALEURS PAR DEFAUT :
 *  - srcRect : texture entière
 *  - center : centre de la texture
 *  - scale : 1x1
 *  - rotation : 0
 *  - non animé // TODO:  faire un autre systeme?
 */
extern void Sprite_set_texture(Sprite *sprite, int renderPriority, int textureID);

/**
 * @brief Configure l'animation d'un sprite.
 *
 * @param sprite sprite à configurer.
 * @param frameCount Nombre total de frames dans l'animation.
 * @param delay délai entre chaque frame de l'animation.
 */
extern void Sprite_set_animation(Sprite *sprite, int frameCount, int delay);

/**
 * @brief Définit le rectangle source pour le sprite.
 *
 * Utilisé pour sélectionner une portion de la texture comme image du sprite.
 *
 * @param sprite sprite à configurer.
 * @param x coordonnée X du coin supérieur gauche dans la texture.
 * @param y coordonnée Y du coin supérieur gauche dans la texture.
 * @param width largeur du rectangle source.
 * @param height hauteur du rectangle source.
 */
extern void Sprite_set_SourceRect(Sprite *sprite, float x, float y, float width, float height);

/**
 * @brief Met à jour l'état d'animation interne d'un sprite.
 *
 * Incrémente le timer d'animation et change la texture actuellement affichée si nécessaire
 *
 * @param sprite Pointeur vers le `Sprite`.
 */
void UpdateAnimation(Sprite *sprite);

/**
 * @brief Dessine un sprite à la position donnée.
 *
 * @param sprite sprite à dessiner
 * @param pos position à laquelle dessinner le sprite
 * @param tag Ce tag est utilisé pour savoir si l'entité doit ignorer la rotation ou non
 */
extern void Sprite_draw_sprite(Sprite *sprite, Position *pos, Tag *tag);

/**
 * @brief Dessine tous les sprites présents dans la pool.
 *
 * Dessine les sprites selon leur
 * renderPriority.
 *
 * @param pool pool courante (toutes les données de l'ECS)
 */
extern void Sprite_draw_all(Pool *pool);

/**
 * @brief Dessine les sprites dont la `renderPriority` est dans l'intervalle.
 *
 * @param p Pointeur vers la `Pool`.
 * @param min_layer Couche minimale (inclusive).
 * @param max_layer Couche maximale (inclusive).
 */
extern void Sprite_draw_range(Pool *p, int min_layer, int max_layer);
