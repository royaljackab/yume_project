#include "../lib/button.h"
#include "../lib/input.h"

#include <raylib.h>

void button_system_init(ButtonSystem *buttonSystem) {
  buttonSystem->nbButtons = 0;
  buttonSystem->selection = 0;
}

void button_create(ButtonSystem *buttonSystem, int x, int y) {
  button_create_ex(buttonSystem, x, y, NO_BUTTON, NO_BUTTON);
}

void button_create_ex(ButtonSystem *buttonSystem, int x, int y, int left,
                      int right) {
  buttonSystem->buttons[buttonSystem->nbButtons].buttonLeft = left;
  buttonSystem->buttons[buttonSystem->nbButtons].buttonRight = right;
  buttonSystem->buttons[buttonSystem->nbButtons].validated = 0;
  buttonSystem->buttons[buttonSystem->nbButtons].pos.x = x;
  buttonSystem->buttons[buttonSystem->nbButtons].pos.y = y;

  buttonSystem->nbButtons++;
}

void button_update(ButtonSystem *buttonSystem, InputSystem *inputState) {

  // Mise a jour du curseur
  if (isPressed(inputState->down) &&
      buttonSystem->selection < buttonSystem->nbButtons - 1) {
    (buttonSystem->selection)++;
  }
  if (isPressed(inputState->up) && buttonSystem->selection > 0) {
    (buttonSystem->selection)--;
  }
  if (isPressed(inputState->left)) {
    if (buttonSystem->buttons[buttonSystem->selection].buttonLeft !=
        NO_BUTTON) {
      buttonSystem->selection =
          buttonSystem->buttons[buttonSystem->selection].buttonLeft;
    }
  }
  if (isPressed(inputState->right)) {
    if (buttonSystem->buttons[buttonSystem->selection].buttonRight !=
        NO_BUTTON) {
      buttonSystem->selection =
          buttonSystem->buttons[buttonSystem->selection].buttonRight;
    }
  }

  // Update les boutons
  buttonSystem->buttons[buttonSystem->selection].validated =
      isPressed(inputState->validate);
}

int button_is_validated(ButtonSystem *buttonSystem) {
  /***
   * Renvoie vrai si le bouton actuel est sélectionné
   */
  return buttonSystem->buttons[buttonSystem->selection].validated;
}

int button_is_button_validated(ButtonSystem *buttonSystem, int buttonID) {
  /***
   * Renvoie vrai si le bouton ID est sélectionné
   */

  return buttonSystem->buttons[buttonID].validated;
}

/* Accès aux champs */

int button_get_current_buttonID(ButtonSystem *buttonSystem) {
  return buttonSystem->selection;
}

Button button_get_current_button(ButtonSystem *buttonSystem) {
  return buttonSystem->buttons[buttonSystem->selection];
}

Vector2 button_get_button_position(ButtonSystem *buttonSystem, int buttonID) {
  return buttonSystem->buttons[buttonID].pos;
}

/* Fonctions utilitaires */
// WARNING: CES FONCTIONS SONT A DEPLACER ! pas fan que bouton gere de
// l'affichage, a deplacer dans render/sprite.h quand ce dernier sera fait.

// NOTE: Cette fonction est vouée a disparaitre (ou pas?), j'espere qu'on aura
// un sprite pour chaque bouton
void button_draw_button_text(ButtonSystem *buttonSystem, int buttonID,
                             char *text, int fontSize, Color color) {
  /***
   * Ecris le texte a la position du bouton
   */

  Vector2 buttonPos = button_get_button_position(buttonSystem, buttonID);
  DrawText(text, buttonPos.x, buttonPos.y, fontSize, color);
}

// TODO: button_draw_sprite quand le systeme de render sera bien fait

// NOTE: De même, bientôt deprecated..
void button_draw_selector_text(ButtonSystem *buttonSystem, int x_offset,
                               int y_offset, char *text, int fontSize,
                               Color color) {
  /***
   * Ecris le texte a la position du curseur, relative aux boutons
   * Utile quand le curseur se comporte de la meme maniere relativement a chaque
   * bouton (ce qui devrait etre la grande grande majorité du cas ?)
   */

  Vector2 offset = {x_offset, y_offset};
  Vector2 pos = Vector2Add(
      offset, button_get_button_position(
                  buttonSystem, button_get_current_buttonID(buttonSystem)));
  DrawText(text, pos.x, pos.y, fontSize, color);
}
