#include "components/common.h"
#include "screen.h"
#include <raylib.h>

bool Position_is_out_of_bounds(Position *pos) {
    Vector2 p = pos->pos;

    return (p.x < PANEL_LEFT - BOUNDS_X || p.x > PANEL_RIGHT + BOUNDS_X 
     || p.y < PANEL_UP - BOUNDS_Y || p.y > PANEL_DOWN + BOUNDS_Y);
}

void Position_set_position(Position *pos, float x, float y) {
    pos->pos = (Vector2){x,y};
}


bool Tag_in_array(Tag * tag, Tag * array, int size) {
    for (int i = 0; i < size; i++) {
        if (*tag == array[i]) {
            return true;
        }
    }
    return false;
}