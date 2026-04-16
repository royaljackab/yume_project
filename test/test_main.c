#include <CUnit/Basic.h>
#include <stdio.h>

// Prototypes des fonctions de tests définies dans les autres fichiers
void test_physics_movement(void);
void test_life_damage(void);

int main() {
    // Initialisation du registre de tests CUnit
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // Suite de tests pour les composants
    CU_pSuite pSuite = CU_add_suite("Suite_Components", NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Ajout des tests individuels à la suite
    if ((NULL == CU_add_test(pSuite, "test of physics movement", test_physics_movement)) ||
        (NULL == CU_add_test(pSuite, "test of life damage", test_life_damage))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Lancer les tests avec l'interface basique
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    
    CU_cleanup_registry();
    return CU_get_error();
}