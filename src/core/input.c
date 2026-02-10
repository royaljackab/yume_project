#include "../../lib/input.h"
#include <raylib.h>

/* Macros */
#define UPDATE_KEY(key) input_update_key(&input->key, cfg->key)

/* Static functions */
static void input_update_key(KeyState *keyState, KeyboardKey key) {
  keyState->isDown = IsKeyDown(key);
  keyState->isPressed = IsKeyPressed(key);
  keyState->isReleased = IsKeyReleased(key);
}

/* Extern functions */

void input_initialize(InputSystem *input) {
  // TODO: Remplir avec settings.txt s'il existe
  input->keybinds.up = KEY_UP;
  input->keybinds.down = KEY_DOWN;
  input->keybinds.left = KEY_LEFT;
  input->keybinds.right = KEY_RIGHT;

  input->keybinds.bomb = KEY_X;
  input->keybinds.shoot = KEY_Z;
  input->keybinds.focus = KEY_LEFT_SHIFT;
  input->keybinds.skip = KEY_LEFT_CONTROL;

  input->keybinds.pause = KEY_P;
  input->keybinds.validate = KEY_SPACE;
}

void input_update(InputSystem *input) {
  /***
   * Chaque frame, remplit l'input pour qu'il corresponde
   * aux actions du joueur
   */

  InputConfig *cfg = &input->keybinds;

  UPDATE_KEY(up);
  UPDATE_KEY(down);
  UPDATE_KEY(left);
  UPDATE_KEY(right);
  UPDATE_KEY(shoot);
  UPDATE_KEY(focus);
  UPDATE_KEY(bomb);
  UPDATE_KEY(skip);
  UPDATE_KEY(pause);
  UPDATE_KEY(validate);
}

int isPressed(KeyState key) { return key.isPressed; }

int isDown(KeyState key) { return key.isDown; }

int isReleased(KeyState key) { return key.isReleased; }

void input_change_keybind(KeyboardKey *key, KeyboardKey new_key) {
  *key = new_key;
}
