#ifndef MENU_H
#define MENU_H

// #include "../include/raylib.h"
#include "globals.h"
#include "input.h"
#include <raylib.h>

#define MAX_BUTTONS 30
#define NO_BUTTON -1

typedef struct Button{
  Vector2 pos;
  int buttonLeft;
  int buttonRight;
  int validated;
} Button;

typedef struct ButtonSystem{
  Button buttons[MAX_BUTTONS];
  int selection;
  int nbButtons;
} ButtonSystem;

extern void button_system_init(ButtonSystem *buttonSystem);
extern void button_update(ButtonSystem *buttonSystem, InputSystem *inputState);

extern int button_is_validated(ButtonSystem *buttonSystem);
extern int button_is_button_validated(ButtonSystem * buttonSystem, int buttonID);
extern void button_create(ButtonSystem *buttonSystem, int x, int y);
extern void button_create_ex(ButtonSystem *buttonSystem, int x, int y, int left, int right);

extern int button_get_current_buttonID(ButtonSystem * buttonSystem);
extern Button button_get_current_button(ButtonSystem * buttonSystem);
extern Vector2 button_get_button_position(ButtonSystem * buttonSystem, int buttonID);

extern void button_draw_button_text(ButtonSystem * buttonSystem, int buttonID, char * text, int fontSize, Color color);
extern void button_draw_selector_text(ButtonSystem * buttonSystem, int x_offset, int y_offset, char * text, int fontSize, Color color);
#endif
