#pragma once

#include "dynarray.h"
#include <stdint.h>

typedef enum CoEventStatus {
    CO_EVENT_PENDING,
    CO_EVENT_SIGNALED,
    CO_EVENT_CANCELED
} CoEventStatus;

typedef struct BoxedTask BoxedTask;

typedef struct CoEvent {
    DYNAMIC_ARRAY(BoxedTask) subscribers; // Tableau des tâches dépendantes de l'evenement
    uint32_t unique_id; // Securite pour la destruction d'evenement
    uint32_t num_signaled; // Nombre de fois ou l'evenement a ete signale
} CoEvent;

/* Photo d'un evenement */
/* Permet de comparer les events au moment de réveiller les taches */
typedef struct CoEventSnapshot {
    uint32_t unique_id;
    uint32_t num_signaled;
} CoEventSnapshot;

// Technique interdite du jujutsu union GPT
#define COEVENTS_ARRAY(...) \
    union { \
        CoEvent _first_event_; \
        struct { CoEvent __VA_ARGS__; }; \
    }

typedef COEVENTS_ARRAY(finished) CoTaskEvents;

#include "cotask.h"

/**
 * @brief Initialise un événement
 * Lui donne un id unique
 * @param evt 
 */
void coevent_init(CoEvent *evt);

void coevent_signal(CoEvent *evt);

/**
 * @brief Prend une "photo" de l'événement
 * @param evt 
 * @return CoEventSnapshot 
 */
CoEventSnapshot coevent_snapshot(const CoEvent *evt);

/**
 * @brief Renvoie l'état de l'evenement par rapport a un snapshot
 * 
 * il faut comprendre que quand on fait dormir une tache sur un evenement,
 * on fait un snap de l'evenement pour s'en souvenir.
 * Plus tard, le sched va verifier si la tache doit se reveiller en comparant le
 * snap a l evenement actuel ie si il a ete signalé alors on aura sur l evenement un
 * num_signal > au num_signal du snap => on reveille la tache!
 *
 * @param evt 
 * @param snap 
 * @return CoEventStatus 
 */
CoEventStatus coevent_poll(const CoEvent *evt, const CoEventSnapshot *snap);

/**
 * @brief Nettoie l'evenement en ne gardant dans ses subscribers bien ceux qui le reference correctement
 * 
 * @param evt 
 */
void coevent_cleanup_subscribers(CoEvent *evt);

/**
 * @brief Ajoute task en subscriber de l'evenement
 * 
 * @param evt 
 * @param task 
 */
void coevent_add_subscriber(CoEvent *evt, CoTask *task);

/**
 * @brief Signale l'evenement et reveille ses enfants
 * 
 * @param evt 
 */
void coevent_signal(CoEvent *evt);

/**
 * @brief Signale l'evenement ssi il n'a jamais ete signale avant
 * 
 * @param evt 
 */
void coevent_signal_once(CoEvent *evt);

/**
 * @brief annule un event et reveille tous ses enfants
 * 
 * @param evt 
 */
void coevent_cancel(CoEvent *evt);

/**
 * @brief Execute func sur chaque evenement d'un tableau de num coevent
 * 
 * @param num 
 * @param events 
 * @param func 
 */
void _coevent_array_action(unsigned int num, CoEvent *events, void (*func)(CoEvent*));

#define COEVENT_ARRAY_ACTION(func, array) \
    (_coevent_array_action(sizeof(array)/sizeof(CoEvent), &((array)._first_event_), func))
#define COEVENT_INIT_ARRAY(array) COEVENT_ARRAY_ACTION(coevent_init, array)
#define COEVENT_CANCEL_ARRAY(array) COEVENT_ARRAY_ACTION(coevent_cancel, array)