/**
 * @file test_physics.c
 * @brief Tests unitaires du composant Physics
 *
 * Teste uniquement Physics_create_speed de manière autonome,
 * sans dépendance à la pool ou à raylib.
 * 
 * @author @amyelalem
 */

#include "test_utils.h"
#include <math.h>

/* Redéfinition minimale pour les tests, sans dépendances externes */
#define NO_MAX_SPEED 999999
#define NO_MIN_SPEED -999999

typedef struct { float x, y; } Vector2;

typedef struct {
    float speed;
    float accel;
    float maxSpd;
    float minSpd;
    float angVel;
    Vector2 velocity;
} Physics;

Physics Physics_create_speed(float speed) {
    Physics phy = {speed, 0, NO_MAX_SPEED, NO_MIN_SPEED, 0, {0, 0}};
    return phy;
}

void test_physics() {
    TEST_SUITE("Physics");

    Physics phy = Physics_create_speed(5.0f);
    TEST("Physics_create_speed : speed initialisee correctement", phy.speed == 5.0f);
    TEST("Physics_create_speed : accel a zero par defaut", phy.accel == 0.0f);
    TEST("Physics_create_speed : angVel a zero par defaut", phy.angVel == 0.0f);
    TEST("Physics_create_speed : maxSpd = NO_MAX_SPEED par defaut", phy.maxSpd == NO_MAX_SPEED);
    TEST("Physics_create_speed : minSpd = NO_MIN_SPEED par defaut", phy.minSpd == NO_MIN_SPEED);
    TEST("Physics_create_speed : velocity.x a zero avant le premier update", phy.velocity.x == 0.0f);
    TEST("Physics_create_speed : velocity.y a zero avant le premier update", phy.velocity.y == 0.0f);

    Physics phy2 = Physics_create_speed(0.0f);
    TEST("Physics_create_speed : vitesse nulle acceptee", phy2.speed == 0.0f);

    Physics phy3 = Physics_create_speed(-3.0f);
    TEST("Physics_create_speed : vitesse negative acceptee", phy3.speed == -3.0f);
}

int main() {
    test_physics();
    printf("\n");
    return 0;
}