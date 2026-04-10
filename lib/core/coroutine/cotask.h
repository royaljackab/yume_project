#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "ecs.h"
#include "../include/koishi.h"
#include "list.h"
#include "common.h"

typedef struct CoTask CoTask;
typedef LIST_ANCHOR(CoTask) CoTaskList;
typedef void *(*CoTaskFunc)(void *arg, size_t argsize);

typedef enum CoStatus{
    CO_STATUS_SUSPENDED = KOISHI_SUSPENDED,
    CO_STATUS_RUNNING = KOISHI_RUNNING,
    CO_STATUS_IDLE = KOISHI_IDLE,
    CO_STATUS_DEAD = KOISHI_DEAD
} CoStatus;

/**
 * @brief Une BoxedTask est une référence faible a une task
 */
typedef struct BoxedTask {
    uintptr_t ptr; // pointeur vers la task
    uint32_t unique_id; // sécurité
} BoxedTask;

#include "coevent.h"

// Pour les wait
typedef struct CoWaitResult {
    int frames;
    CoEventStatus event_status;
} CoWaitResult;

typedef struct CoSched CoSched;

/*----------------------------------*/
/* Fonctions */
/*----------------------------------*/

void cotask_cancel(CoTask *task);

/**
 * @brief Emballer une task dans une boite (ref faible)
 * 
 * @param task 
 * @return BoxedTask 
 */
BoxedTask cotask_box(CoTask *task);

/**
 * @brief A partir d'une boite, retrouver la task correspondante
 * 
 * Si l'id de la boite ne correspond pas a l'id de la task,
 * alors cette derniere a été recyclée et on renvoie NULL
 *
 * @param box 
 * @return CoTask* 
 */
CoTask *cotask_unbox(BoxedTask box);

/**
 * @brief Annule une tache
 * 
 * @param task 
 */
void cotask_cancel(CoTask *task);

/**
 * @brief Renvoie le statut de la task
 * 
 * @param task 
 * @return CoStatus 
 */
CoStatus cotask_status(CoTask *task);

/**
 * @brief Renvoie la cotask actuellement en train de run
 * 
 * @return CoTask* 
 */
CoTask *cotask_active(void);

/**
 * @brief Redonne la main a la task
 * 
 * @param task 
 * @param arg 
 * @return void* 
 */
void *cotask_resume(CoTask *task, void *arg);

/**
 * @brief Yield la coroutine active (redonne la main au main)
 * 
 * @param arg 
 * @return void* 
 */
void *cotask_yield(void *arg);

/**
 * @brief Permet de yield pdt delay frames
 * 
 * @param delay 
 * @return int nombre de frames restantes
 */
int cotask_wait(int delay);

/**
 * @brief Wait les soustaches actives
 * 
 * @return int 
 */
int cotask_wait_subtasks(void);

/**
 * @brief Wait un événement
 * 
 * @param evt 
 * @return CoWaitResult 
 */
CoWaitResult cotask_wait_event(CoEvent *evt);

/**
 * @brief Wait un événement une seule fois
 * 
 * @param evt 
 * @return CoWaitResult 
 */
CoWaitResult cotask_wait_event_once(CoEvent *evt);

/**
 * @brief Wait un événement, sinon cancel la tache
 * 
 * @param evt 
 * @return CoWaitResult 
 */
CoWaitResult cotask_wait_event_or_die(CoEvent *evt);

/**
 * @brief Renvoie le scheduler d'une tache
 * 
 * @param task 
 * @return CoSched* 
 */
CoSched *cotask_get_sched(CoTask *task);

/**
 * @brief Rattache une tache a une entité
 * 
 * @param task 
 * @param ent 
 * @return Entity 
 */
Entity cotask_bind_to_entity(CoTask *task, Entity ent);

/**
 * @brief Renvoie les événements d'une tache
 * 
 * @param task 
 * @return CoTaskEvents* 
 */
CoTaskEvents *cotask_get_events(CoTask *task);


Entity cotask_host_entity(CoTask *task, Tag ent_type);
void cotask_host_events(CoTask *task, unsigned int num_events, CoEvent events[num_events]);

void cotask_free(CoTask *task);

void cotask_force_finish(CoTask *task);
