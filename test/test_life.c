/**
 * @file test_life.c
 * @brief Tests unitaires du composant Life
 *
 * Ce fichier teste les fonctions de gestion des points de vie.
 * Le composant Life est utilisé par les ennemis et le boss pour
 * savoir quand ils doivent mourir.
 *
 * Fonctions testées :
 *   - Life_damage  : inflige des dégâts (soustrait des PV)
 *   - Life_heal    : soigne (ajoute des PV)
 *   - Life_is_dead : renvoie true si les PV sont <= 0
 *
 * Cas limites testés :
 *   - Dégâts supérieurs aux PV restants (vie négative)
 *   - Vie exactement à zéro
 *   - Soin sur une entité blessée
 * 
 * @author @amyelalem
 */

#include "test_utils.h"
#include "../lib/components/life.h"

void test_life() {
    TEST_SUITE("Life");

    /* --- Life_damage --- */
    /* Cas normal : on inflige 3 dégâts sur 10 PV */
    Life l = {10, 10};
    Life_damage(&l, 3);
    TEST("Life_damage : soustrait les degats correctement (10 - 3 = 7)", l.life == 7);

    /* La vie doit pouvoir tomber exactement à zéro */
    Life_damage(&l, 7);
    TEST("Life_damage : vie tombe exactement a zero", l.life == 0);

    /* --- Life_is_dead --- */
    /* Une entité avec 0 PV doit être considérée comme morte */
    TEST("Life_is_dead : mort quand vie == 0", Life_is_dead(&l) == true);

    /* Une entité avec des PV restants ne doit pas être morte */
    Life l2 = {10, 5};
    TEST("Life_is_dead : vivant quand vie > 0", Life_is_dead(&l2) == false);

    /* --- Life_heal --- */
    /* Soin normal : 5 PV + 3 soins = 8 PV */
    Life_heal(&l2, 3);
    TEST("Life_heal : ajoute les PV correctement (5 + 3 = 8)", l2.life == 8);

    /* --- Cas limite : dégâts supérieurs aux PV restants --- */
    /* La vie peut passer en négatif, c'est voulu (pas de clamp) */
    Life l3 = {10, 10};
    Life_damage(&l3, 15);
    TEST("Life_damage : vie passe en negatif si degats > PV restants", l3.life == -5);
    TEST("Life_is_dead : mort quand vie < 0", Life_is_dead(&l3) == true);
}

int main() {
    test_life();
    printf("\n");
    return 0;
}