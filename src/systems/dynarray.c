#include "systems/dynarray.h"

#include <assert.h>
#include <string.h>

void _dynarray_free_data(dynarray_size_t sizeof_element, DynamicArray *darr) {
    free(darr->data);
    *darr = (typeof(*darr)) {}; //Tableau vide
}

/**
 * @brief Change la taille d'un dynarray en capacity
 * 
 * @param sizeof_element taille d'un element
 * @param darr 
 * @param capacity taille cible
 */
static
void _dynarray_resize(dynarray_size_t sizeof_element, DynamicArray *darr, dynarray_size_t capacity) {
    assert(sizeof_element > 0);
    assert(capacity > 0);
    darr->capacity = capacity;
    darr->data = realloc(darr->data, sizeof_element * capacity);
}

void _dynarray_ensure_capacity(dynarray_size_t sizeof_element, DynamicArray *darr, dynarray_size_t capacity) {
    if (darr->capacity < capacity) {
        _dynarray_resize(sizeof_element, darr, capacity);
    }
}

dynarray_size_t _dynarray_prepare_append_min_capacity(dynarray_size_t sizeof_element, DynamicArray *darr, dynarray_size_t min_capacity) {
    dynarray_size_t nb_elem = darr->num_elements;
    dynarray_size_t capacity = darr->capacity;

    // On vérifie qu'il y a bien au moins la place minimale. Sinon on resize
    if (capacity < min_capacity) {
        _dynarray_resize(sizeof_element, darr, min_capacity);
    } else if (capacity == nb_elem) {
        capacity += capacity >> 1; // c = c + c / 2
        _dynarray_resize(sizeof_element, darr, capacity);
    }

    (darr->num_elements)++;

    return nb_elem; // Indice d'insertion
}

void _dynarray_compact(dynarray_size_t sizeof_element, DynamicArray *darr) {
    if (darr->capacity > darr->num_elements) {
        dynarray_size_t newsize = darr->num_elements;

        if (newsize < 1) {
            newsize = 1;
            if (darr->capacity == newsize) return; // Tableau vide
        }

        _dynarray_resize(sizeof_element, darr, newsize);
    }
}

void _dynarray_set_elements(dynarray_size_t sizeof_element, DynamicArray *darr, dynarray_size_t num_elements, void *elements) {
    _dynarray_ensure_capacity(sizeof_element, darr,num_elements);
    memcpy(darr->data, elements, sizeof_element * num_elements);
    darr->num_elements = num_elements;
}

void _dynarray_filter(dynarray_size_t sizeof_element, DynamicArray *darr, dynarray_filter_t filter, void *userdata) {
    if(!darr->data)
        return;

    char *p = darr->data;
    char *end = p + darr->num_elements * sizeof_element;
    dynarray_size_t shift = 0;

    while (p < end) {
        if (!filter(p, userdata)) {
            shift += sizeof_element;
            darr->num_elements--;
        } else if (shift) {
            memmove(p - shift, p, sizeof_element);
        }

        p += sizeof_element;
    }
}

