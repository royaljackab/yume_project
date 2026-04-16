#pragma once

// #include "../include/raylib.h"
#include "screen.h"
#include "input.h"
#include <raylib.h>

#define MAX_BUTTONS 30
#define NO_BUTTON -1

typedef struct Button {
  Vector2 pos;
  int buttonLeft;
  int buttonRight;
  int validated;
} Button;

typedef struct ButtonSystem {
  Button buttons[MAX_BUTTONS];
  int selection;
  int nbButtons;
} ButtonSystem;


/** @brief Initialise le système de boutons
 * @param buttonSystem Le système de boutons à initialiser
 */
extern void button_system_init(ButtonSystem *buttonSystem);

/** @brief Met à jour le système de boutons en fonction de l'état des entrées
 * @param buttonSystem Le système de boutons à mettre à jour
 * @param inputState L'état des entrées à prendre en compte pour la mise à jour
 */
extern void button_update(ButtonSystem *buttonSystem, InputSystem *inputState);

/** @brief Vérifie si le bouton actif a été validé
 * @param buttonSystem Le système de boutons
 * @return 1 si le bouton actif a été validé, 0 sinon
 */
extern int button_is_validated(ButtonSystem *buttonSystem);

/** @brief Vérifie si un bouton spécifique a été validé
 * @param buttonSystem Le système de boutons
 * @param buttonID L'ID du bouton à vérifier
 * @return 1 si le bouton a été validé, 0 sinon
 */
extern int button_is_button_validated(ButtonSystem *buttonSystem, int buttonID);

/** @brief Crée un nouveau bouton
 * @param buttonSystem Le système de boutons
 * @param x La position X du bouton
 * @param y La position Y du bouton
 */
extern void button_create(ButtonSystem *buttonSystem, int x, int y);

/** @brief Crée un nouveau bouton avec des touches spécifiques
 * @param buttonSystem Le système de boutons
 * @param x La position X du bouton
 * @param y La position Y du bouton
 * @param left Le code de la touche de gauche
 * @param right Le code de la touche de droite
 */
extern void button_create_ex(ButtonSystem *buttonSystem, int x, int y, int left,
                             int right);

/** @brief Récupère l'ID du bouton actuellement sélectionné
 * @param buttonSystem Le système de boutons à dessiner
 */
extern int button_get_current_buttonID(ButtonSystem *buttonSystem);

/** @brief Récupère le bouton actuellement sélectionné (retourne la structure complète du bouton)
 * @param buttonSystem Le système de boutons à dessiner
 * @return Le bouton actuellement sélectionné
 */
extern Button button_get_current_button(ButtonSystem *buttonSystem);

/** @brief Récupère la position d'un bouton spécifique
 * @param buttonSystem Le système de boutons
 * @param buttonID L'ID du bouton dont on veut récupérer la position
 * @return La position du bouton spécifié
 */
extern Vector2 button_get_button_position(ButtonSystem *buttonSystem,
                                          int buttonID);

/** @brief Dessine le texte d'un bouton spécifique
 * @param buttonSystem Le système de boutons
 * @param buttonID L'ID du bouton dont on veut dessiner le texte
 * @param text Le texte à dessiner
 * @param fontSize La taille de la police
 * @param color La couleur du texte
 */
extern void button_draw_button_text(ButtonSystem *buttonSystem, int buttonID,
                                    char *text, int fontSize, Color color);

/** @brief Dessine le texte du sélecteur de bouton avec la police "Touhou98"
 * @param buttonSystem Le système de boutons
 * @param x_offset Le décalage X du texte du sélecteur
 * @param y_offset Le décalage Y du texte du sélecteur
 * @param text Le texte à dessiner
 * @param fontSize La taille de la police
 * @param color La couleur du texte
 */
extern void button_draw_button_text_touhou98(ButtonSystem *buttonSystem, int buttonID, char *text, int fontSize, Color color);

/** @brief Dessine le texte du sélecteur de bouton
 * @param buttonSystem Le système de boutons
 * @param x_offset Le décalage X du texte du sélecteur
 * @param y_offset Le décalage Y du texte du sélecteur
 * @param text Le texte à dessiner
 * @param fontSize La taille de la police
 * @param color La couleur du texte
 */
extern void button_draw_selector_text(ButtonSystem *buttonSystem, int x_offset,
                                      int y_offset, char *text, int fontSize,
                                      Color color);
