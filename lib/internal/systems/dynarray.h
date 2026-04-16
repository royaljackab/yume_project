#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "core/macro.h"

typedef uint32_t dynarray_size_t;

#define auto __auto_type

/**
 * @brief base d'un tableau dynamique
 * 
 * Un tableau dynamique homogène avec son type
 * num_elements -> nombre d'elem courrants
 * capacity -> taille max du tableau
 */
#define DYNAMIC_ARRAY_BASE(type_elem) struct { \
    type_elem *data; \
    dynarray_size_t num_elements; \
    dynarray_size_t capacity; \
}

/**
 * @brief Tableau dynamique générique (data est un pointeur universel)
 * 
 * @return typedef 
 */
typedef DYNAMIC_ARRAY_BASE(void) DynamicArray;

/**
 * @brief Déclaration d'un tableau dynamique
 * 
 * Petit tricks de batard pour a la fois avoir une sécurité sur le type
 * du tableau et en meme temps pouvoir utiliser les fonctions génériques
 */
#define DYNAMIC_ARRAY(elem_type) union { \
    DYNAMIC_ARRAY_BASE(elem_type); \
    DynamicArray din_array; \
}

/**
 * @brief Renvoie pointeur vers le tableau générique
 */
#define DYNARRAY_CAST_TO_BASE(darr) &(darr)->din_array

/**
 * @brief Renvoie la taille d'un élément du tableau
 */
#define DYNARRAY_ELEM_SIZE(darr) sizeof(*(darr)->data)

/**
 * @brief "Traducteurs" pour les fonctions dynarray
 *
 * Comme a chaque fois il faut passer en param la taille d'un element et le pointeur vers le tableau
 * cette macro gère ça toute seule sinon c chiant d'écrire a la main a chaque fois
 */
#define _dynarray_func(func, darr, ...) \
    _dynarray_##func(DYNARRAY_ELEM_SIZE(darr), DYNARRAY_CAST_TO_BASE(darr), __VA_ARGS__)
#define _dynarray_func_noargs(func, darr) \
    _dynarray_##func(DYNARRAY_ELEM_SIZE(darr), DYNARRAY_CAST_TO_BASE(darr))

/**
 * @brief Libère la mémoire du data d'un dynarray
 * 
 * @param sizeof_element 
 * @param darr 
 */
void _dynarray_free_data(dynarray_size_t sizeof_element, DynamicArray *darr);
#define dynarray_free_data(darr) _dynarray_func_noargs(free_data, darr)

/**
 * @brief Fait en sorte que la capacité du tableau soit au moins capacity
 * 
 * @param sizeof_element 
 * @param darr 
 * @param capacity 
 */
void _dynarray_ensure_capacity(dynarray_size_t sizeof_element, DynamicArray *darr, dynarray_size_t capacity);
#define dynarray_ensure_capacity(darr, capacity) _dynarray_func(ensure_capacity, darr, capacity)

/**
 * @brief Vérifie qu'il y a la capacité nécessaire pour append et en alloue si besoin
 * 
 * @param sizeof_element 
 * @param darr 
 * @param min_capacity 
 * @return dynarray_size_t : Indice d'insertion
 */
dynarray_size_t _dynarray_prepare_append_min_capacity(dynarray_size_t sizeof_element, DynamicArray *darr, dynarray_size_t min_capacity);

/**
 * @brief Renvoie le pointeur vers data[idx]
 */
#define dynarray_get_ptr(darr, idx) ({ \
    (darr)->data + idx; \
})

#define _dynarray_append_min_capacity(darr, min_capacity, ...) ({ \
    auto elem_p = dynarray_get_ptr(darr, _dynarray_func(prepare_append_min_capacity, darr, min_capacity)); \
    __VA_OPT__(*elem_p = ((typeof(*elem_p)) __VA_ARGS__); ) \
    elem_p; \
})

#define dynarray_append(darr, ...) \
    _dynarray_append_min_capacity(darr, 2, ##__VA_ARGS__);

/**
 * @brief Fait en sorte qu'il n'y ait pas de mémoire vide dans data
 * 
 * @param sizeof_element 
 * @param darr 
 */
void _dynarray_compact(dynarray_size_t sizeof_element, DynamicArray *darr);
#define dynarray_compact(darr) _dynarray_func_noargs(compact, darr)

/**
 * @brief Trie le tableau de data par un qsort
 */
#define dynarray_qsort(darr, compare_func) do { \
    DynamicArray *b_darr = DYNARRAY_CAST_TO_BASE(darr); \
    if (b_darr->data) { \
        qsort(b_darr->data, b_darr->num_elements, DYNARRAY_ELEM_SIZE(darr), (compare_func)); \
    } \
} while(0)

/**
 * @brief Renvoie la valeur de data[idx]
 */
#define dynarray_get(darr, idx) (*dynarray_get_ptr(darr, idx))
/**
 * @brief Modifie la valeur de data[idx]
 */
#define dynarray_set(darr,idx,...) (*dynarray_get_ptr(darr, idx) = (__VA_ARGS__)) 

/**
 * @brief Rentre un tableau d'elements dans le tableau dynamique
 * 
 * @param sizeof_element 
 * @param darr 
 * @param num_elements nombre d'éléments a insérer
 * @param elements tableau d'éléments
 */
void _dynarray_set_elements(dynarray_size_t sizeof_element, DynamicArray *darr, dynarray_size_t num_elements, void *elements);
// TODO: Failsafe si ce n'est pas le bon type d'éléments ? ou je vous fais confiance :)
#define dynarray_set_elements(darr, num_elements, elements) _dynarray_func(set_elements, darr, num_elements, elements)

/**
 * @brief Fonction qui filtre un tableau
 */
typedef bool (*dynarray_filter_t)(const void *elem_p, void *userdata);

/**
 * @brief Modifie un tableau pour ne garder que les valeurs respectant le filtre
 * 
 * @param sizeof_element 
 * @param darr 
 * @param filter 
 * @param userdata 
 */
void _dynarray_filter(dynarray_size_t sizeof_element, DynamicArray *darr, dynarray_filter_t predicate, void *userdata);
#define dynarray_filter(darr, predicate, userdata) _dynarray_func(filter, darr, predicate, userdata)

/**
 * @brief Renvoie l'indice d'elem dans le tableau
 */
#define dynarray_indexof(darr, elem_p) ({ \
    int idx = (int)(elem_p - darr->data;) \
    (dynarray_size_t)idx; \
})

/**
 * @brief Pour créer des variables itérables a priori uniques
 * (sinon les boucles imbriquées flop)
 */
#define _dynarray_foreach_iter MACRO_ADDLINENUM(dynarray_foreach_iter)
#define _dynarray_foreach_temp MACRO_ADDLINENUM(dynarray_foreach_temp)

/**
 * @brief Boucle sur le tableau avec var = id, elem_var = pointeur sur data[idx]
 */
#define dynarray_foreach(darr, var, elem_var, ...) do { \
    for (dynarray_size_t _dynarray_foreach_iter = 0; \
        _dynarray_foreach_iter < (darr)->num_elements; _dynarray_foreach_iter++) { \
            dynarray_size_t var = _dynarray_foreach_iter; \
            elem_var = dynarray_get_ptr(darr, var); \
            __VA_ARGS__; \
        } \
} while(0)

/**
 * @brief boucle sur le tableau avec elem_var = pointeur sur les éléments
 */
#define dynarray_foreach_elem(darr, elem_var, ...) \
    dynarray_foreach(darr, \
        _dynarray_foreach_temp, \
        elem_var, \
        __VA_ARGS__ \
    )

/**
 * @brief boucle sur le tableau avec var = id
 */
#define dynarray_foreach_idx(darr, var, ...) \
    dynarray_foreach(darr, \
        var, \
        _dynarray_foreach_temp, \
        __VA_ARGS__ \
    )

