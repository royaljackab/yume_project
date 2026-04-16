#include <CUnit/Basic.h>
#include "ecs/pool.h"
#include "components/life.h"

void test_life_damage(void) {
    Pool *pool = malloc(sizeof(Pool));
    pool_init(pool);

    Entity e = pool_create_entity(pool);
    
    // Ajout de 100 HP
    Life life = {100, 100};
    Life_add(&pool->life, e, life);

    Life *l = Life_get(&pool->life, e);
    Life_damage(l, 30); 

    CU_ASSERT_EQUAL(l->life, 70);
    
    Life_damage(l, 80);
    CU_ASSERT_TRUE(l->life <= 0);
    CU_ASSERT_TRUE(Life_is_dead(l));

    free(pool);
}