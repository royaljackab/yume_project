#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "ecs/pool.h"
#include "components/physics.h"
#include "components/common.h"
#include "obj.h"
#include "systems/screen.h" // Nécessaire pour accéder à la structure Screen

// On récupère la variable globale de ton moteur
extern ScreenSystem *g_screen;

void test_physics_movement(void) {
    ScreenSystem dummy_screen;
    screen_system_init(&dummy_screen);
    g_screen = &dummy_screen;

    Pool *pool = malloc(sizeof(Pool));
    CU_ASSERT_PTR_NOT_NULL_FATAL(pool); // Stoppe le test si la RAM est pleine
    pool_init(pool);

    Entity e = pool_create_entity(pool);
    
    Position pos = {{0.0f, 0.0f}, 0.0f};
    Position_add(&pool->position, e, pos);
    
    Physics phys = Physics_create_speed(10);
    Physics_add(&pool->physics, e, phys);

    Physics_update_all(pool);

    Position *new_pos = Position_get(&pool->position, e);
    
    // Vérification : l'objet a avancé vers la droite a une vitesse de 10
    CU_ASSERT_DOUBLE_EQUAL(new_pos->pos.x, 10.0f, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(new_pos->pos.y, 0.0f, 0.0001);

    // revient a (0,0)
    obj_SetPosition(pool, e, 0, 0);
    obj_SetAngle(pool, e, 45);
    obj_SetSpeed(pool, e, 5);

    Physics_update_all(pool);

    new_pos = Position_get(&pool->position, e);

    // Verif : l'objet a avancé dans la diagonale
    CU_ASSERT_DOUBLE_EQUAL(new_pos->pos.x, 5 * cosf(45 * DEG2RAD), 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(new_pos->pos.y, 5 * sinf(45 * DEG2RAD), 0.0001);

    // 3. Nettoyage
    free(pool);
}