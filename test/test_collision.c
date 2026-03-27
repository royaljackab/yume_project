#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <raylib.h>

int main(void)
{
    Vector2 rectPos = {0, 0};
    float w = 100, h = 100;

    // Cas 1 : cercle au centre → collision
    assert(CheckCircleRotatedRect((Vector2){0, 0}, 10, rectPos, w, h, 0) == true);

    //Cas 2 : cercle loin → pas de collision
    assert(CheckCircleRotatedRect((Vector2){200, 200}, 10, rectPos, w, h, 0) == false);

    // Cas 3 : touche le bord
    assert(CheckCircleRotatedRect((Vector2){50, 0}, 10, rectPos, w, h, 0) == true);

    // Cas 4 : rotation 45° avec collision
    assert(CheckCircleRotatedRect((Vector2){30, 0}, 20, rectPos, w, h, 45) == true);

    // Cas 5 : rotation 45° sans collision
    assert(CheckCircleRotatedRect((Vector2){150, 0}, 10, rectPos, w, h, 45) == false);

    // Cas 6 : coin du rectangle
    assert(CheckCircleRotatedRect((Vector2){70, 70}, 15, rectPos, w, h, 0) == true);

    return 0;
}

