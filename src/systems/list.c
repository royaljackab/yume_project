#include "list.h"

#include <stdlib.h>

#undef list_insert
List* list_insert(List** dest, List* elem) {
    elem->prev = *dest;

    if (*dest != NULL) {
        elem->next = (*dest)->next;

        if ((*dest)->next) {
            (*dest)->next->prev = elem;
        }

        (*dest)->next = elem;
    }

    return elem;
}

#undef alist_insert
List* alist_insert(ListAnchor *list, List *ref, List *elem) {
    elem->prev = ref;

    if (ref != NULL) {
        elem->next = ref->next;

        if (elem->next != NULL) {
            elem->next->prev = elem;
        }

        if (ref == list->last) {
            list->last = elem;
        }

        ref->next = elem;
    } else {
        elem->next = elem->prev = NULL;
        list->first = list->last = elem;
    }

    return elem;
}

#undef list_push
List* list_push(List **dest, List *elem) {
    if(*dest) {
        (*dest)->prev = elem;
    }

    elem->next = *dest;
    elem->prev = NULL;

    *dest = elem;
    return elem;
}

#undef alist_push
List* alist_push(ListAnchor * list, List * elem) {
    elem->next = list->first;
    elem->prev = NULL;

    if (list->last == NULL) {
        // Normalement list->first == NULL alors donc liste vide
        list->last = elem;
    } else {
        list->first->prev = elem;
    }

    list->first = elem;
    return elem;
}

#undef list_append
List* list_append(List **dest, List *elem) {
    if (*dest == NULL) {
        return list_insert(dest, elem);
    }

    // On place un pointeur end a la fin de la liste
    List *end = *dest;
    for(List *e = (*dest)->next; e; e = e->next) {
        end = e;
    }

    return list_insert(&end, elem);
}

#undef alist_append
List* alist_append(ListAnchor * list, List *elem) {
    elem->next = NULL;
    elem->prev = list->last;

    if (list->last == NULL) {
        list->first = elem;
    } else {
        list->last->next = elem;
    }

    list->last = elem;
    return elem;
}

#undef list_unlink
List* list_unlink(List **dest, List *elem) {
    if (elem->prev != NULL) {
        elem->prev->next = elem->next;
    }
    if (elem->next != NULL) {
        elem->next->prev = elem->prev;
    }

    if (*dest == elem) {
        *dest = elem->next;
    }

    return elem;
}

#undef alist_unlink
List* alist_unlink(ListAnchor * list, List* elem) {
    if (list->last == elem) {
        list->last = list->last->prev;
    }

    return list_unlink(&list->first, elem);
}

#undef list_pop
List* list_pop(List **dest) {
    if (*dest == NULL) return NULL;

    return list_unlink(dest, *dest);
}

#undef alist_pop
List* alist_pop(ListAnchor * list) {
    if (list->first == NULL) return NULL;

    return alist_unlink(list, list->first);
}

#undef alist_merge_tail
void alist_merge_tail(ListAnchor *dest, ListAnchor *src) {
    if (src->first) {
        src->first->prev = dest->last;

        if (dest->last) {
            dest->last->next = src->first;
        } else {
            dest->first = src->first;
        }

        dest->last = src->last;
        src->first = NULL;
        src->last = NULL;
    }
}