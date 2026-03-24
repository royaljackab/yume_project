/**
 * @file test_utils.h
 * @brief Utilitaires pour les tests unitaires
 */

#pragma once

#include <stdio.h>
#include <assert.h>

/** @brief Affiche OK si le test passe */
#define TEST(nom, condition) \
    do { \
        if (condition) { \
            printf("[OK]   %s\n", nom); \
        } else { \
            printf("[FAIL] %s\n", nom); \
        } \
    } while(0)

/** @brief Affiche le titre d'une suite de tests */
#define TEST_SUITE(nom) \
    printf("\n--- %s ---\n", nom) 