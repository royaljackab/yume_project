#pragma once

#include "coroutine/coevent.h"
#include "cotask.h"
#include "ecs.h"
#include "list.h"

#include "../include/koishi.h"
#include <stdint.h>
#include <sys/types.h>

enum {
    COTASK_WAIT_NONE,
    COTASK_WAIT_DELAY,
    COTASK_WAIT_EVENT,
    COTASK_WAIT_SUBTASKS,
};

#define CO_STACK_SIZE (256 * 1024)

typedef struct CoTaskData CoTaskData;

// "coquille" d'une tache
struct CoTask {
    LIST_INTERFACE(CoTask);
    koishi_coroutine_t ko; // Emplacement dans la coroutine dans un endroit de la mémoire bresom
    // JSP comment ça marche c géré par koishi

    CoTaskData *data; // Pointeur vers le "cerveau"

    uint32_t unique_id;
    const char * name; // Debug ?
};

// "cerveau" de la tache
struct CoTaskData {
    LIST_INTERFACE(CoTaskData);

    CoTask *task; // pointeur vers sa coquille
    CoSched *sched;

    CoTaskData *master; // Le parent
    LIST_ANCHOR(CoTaskData) slaves; // Les enfants

    BoxedEntity bound_ent;
    CoTaskEvents events;

    bool finalizing;

    // bloc pour les wait
    struct {
        CoWaitResult result;

        union {
            // quand on fait des WAIT(60)
            struct {
                int remaining;
            } delay;

            // pour les WAIT_EVENT(..)
            struct {
                CoEvent *p_event;
                CoEventSnapshot snapshot;
            } event;
        };

        uint wait_type;
    } wait;

    // Entité/events que la tache crée
    struct {
        Entity ent;
        CoEvent *events;
        uint num_events;
    } hosted;
};

typedef struct CoTaskInitData {
    CoTask *task;
    CoSched *sched;
    CoTaskFunc func;
    void *func_arg;
    size_t func_arg_size;
    CoTaskData *master_task_data;
} CoTaskInitData;

void cotask_global_init(void);
void cotask_global_shutdown(void);

/**
 * @brief Pour créer une nouvelle cotask
 * Fonction de "bas" niveau
 * @param entry_point 
 * @return CoTask* 
 */
CoTask *cotask_new_internal(koishi_entrypoint_t entry_point);

/**
 * @brief EHH en gros ce qui fait touner la tache
 * 
 * @param varg 
 * @return void* 
 */
void *cotask_entry(void *varg);

/**
 * @brief Fonction interne pour le resume
 * 
 * @param task 
 * @param arg 
 * @return void* 
 */
void *cotask_resume_internal(CoTask *task, void *arg);

static
inline CoTaskData *cotask_get_data(CoTask *task) {
    return task->data;
}