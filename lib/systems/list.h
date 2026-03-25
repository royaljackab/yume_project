#pragma once

#include "dynarray.h"
typedef struct ListInterface ListInterface;
typedef struct List List;
typedef struct ListAnchorInterface ListAnchorInterface;
typedef struct ListAnchor ListAnchor;
typedef struct ListContainer ListContainer;

#define LIST_INTERFACE_BASE(type) struct { \
    type *next; \
    type *prev; \
}

#define LIST_INTERFACE(type) union { \
    ListInterface list_interface; \
    LIST_INTERFACE_BASE(type); \
}

struct ListInterface {
    LIST_INTERFACE_BASE(ListInterface);
};

struct List {
    LIST_INTERFACE(List);
};

#define LIST_ANCHOR_INTERFACE_BASE(type) struct { \
    type *first; \
    type *last; \
}

#define LIST_ANCHOR_INTERFACE(type) union { \
    ListAnchorInterface list_anchor_interface; \
    LIST_ANCHOR_INTERFACE_BASE(type); \
}

#define LIST_ANCHOR(type) struct { \
    LIST_ANCHOR_INTERFACE(type); \
}

struct ListAnchorInterface {
    LIST_ANCHOR_INTERFACE_BASE(ListInterface);
};

struct ListAnchor {
    LIST_ANCHOR_INTERFACE(List);
};

struct ListContainer {
    LIST_INTERFACE(ListContainer);
    void *data;
};

/*--------------------------------------*/
/* Fonctions de Listes */
/*--------------------------------------*/

/**
 * @brief Insere elem apres dest
 * 
 * @param dest 
 * @param elem 
 * @return List* : elem
 */
List* list_insert(List **dest, List *elem);

/**
 * @brief Insere elem en tête de dest. *dest pointe sur la nouvelle tete.
 * 
 * @param dest 
 * @param elem 
 * @return List* : elem
 */
List* list_push(List **dest, List *elem);

/**
 * @brief Insere elem a la fin de dest.
 * 
 * @param dest 
 * @param elem 
 * @return List* 
 */
List* list_append(List **dest, List *elem);

/**
 * @brief Detache elem de dest
 * 
 * @param dest 
 * @param elem 
 * @return List* : elem
 */
List* list_unlink(List **dest, List *elem);

/**
 * @brief Retire de list son premier element
 * 
 * @param list 
 * @return List* : Premier élément détaché
 */
List* list_pop(List **list);

/*--------------------------------------*/
/* Fonctions de Listes anchor */
/*--------------------------------------*/

/**
 * @brief Insere elem apres ref, un element de list
 * 
 * Si ref = NULL => list vide donc elem devient le seul elem de la list
 * 
 * @param list 
 * @param ref 
 * @param elem 
 * @return List* : elem
 */
List* alist_insert(ListAnchor * list, List * ref, List * elem);

/**
 * @brief Insere elem en tête de list (avant list->first)
 * 
 * @param list 
 * @param elem 
 * @return List* : elem
 */
List* alist_push(ListAnchor * list, List * elem);

/**
 * @brief Insere elem a la fin de list (apres list->last)
 * 
 * @param list 
 * @param elem 
 * @return List* 
 */
List* alist_append(ListAnchor * list, List * elem);

/**
 * @brief Detache elem de list
 * 
 * @param list 
 * @param elem 
 * @return List* : element détaché
 */
List* alist_unlink(ListAnchor * list, List * elem);

/**
 * @brief Retire le premier element de list
 * 
 * @param list 
 * @param elem 
 * @return List* : le premier élément détaché
 */
List* alist_pop(ListAnchor * list);

/**
 * @brief Accroche src a la fin de dest
 * 
 * @param dst 
 * @param src 
 */
void alist_merge_tail(ListAnchor * dst, ListAnchor * src);

/*Permet de convertir les listes en List génériques*/
#define LIST_CAST(expr) (List *)(expr)
#define LIST_CAST_2(expr) (List **)(expr)
#define LIST_ANCHOR_CAST(expr) (ListAnchor *)(expr)
#define LIST_CAST_RETURN(typekey, e_return) (__typeof__(*(typekey)) *)(e_return)

/*--------------------------------------*/
/* Macros des fonctions pour listes (anchor) génériques */
/*--------------------------------------*/

#define list_insert(dest, elem) \
    (LIST_CAST_RETURN(elem, list_insert(LIST_CAST_2(dest), LIST_CAST(elem))))

#define alist_insert(list, ref, elem) \
    (LIST_CAST_RETURN(elem, alist_insert(LIST_ANCHOR_CAST(list), LIST_CAST(ref), LIST_CAST(elem))))

#define list_push(dest, elem) \
    (LIST_CAST_RETURN(elem, list_push(LIST_CAST_2(dest), LIST_CAST(elem))))

#define alist_push(list, elem) \
    (LIST_CAST_RETURN(elem, alist_push(LIST_ANCHOR_CAST(list), LIST_CAST(elem))))

#define list_append(dest, elem) \
    (LIST_CAST_RETURN(elem, list_append(LIST_CAST_2(dest), LIST_CAST(elem))))

#define alist_append(list, elem) \
    (LIST_CAST_RETURN(elem, alist_append(LIST_ANCHOR_CAST(list), LIST_CAST(elem))))

#define list_unlink(dest, elem) \
    (LIST_CAST_RETURN(elem, list_unlink(LIST_CAST_2(dest), LIST_CAST(elem))))

#define alist_unlink(list, elem) \
    (LIST_CAST_RETURN(elem, alist_unlink(LIST_ANCHOR_CAST(list), LIST_CAST(elem))))

#define list_pop(dest) \
    (LIST_CAST_RETURN(*(dest), list_pop(LIST_CAST_2(dest))))

#define alist_pop(list) \
    (LIST_CAST_RETURN((list)->first, alist_pop(LIST_ANCHOR_CAST(list))))

#define alist_merge_tail(dst, src) \
    alist_merge_tail(LIST_ANCHOR_CAST(dst), LIST_ANCHOR_CAST(src))