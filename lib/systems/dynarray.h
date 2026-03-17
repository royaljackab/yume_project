#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

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
#define DYNARRAY_CAST_TO_BASE(darr) &darr->din_array;

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
    assert(idx >= 0); \
    assert(idx < darr->num_elements); \
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