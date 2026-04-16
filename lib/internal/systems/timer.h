#pragma once

#include <stdbool.h>
#include <stdlib.h>

#define MAX_TIMES 100

typedef struct{
    int chrono;
    int nbTime;
    int time[MAX_TIMES];
} Timer;


/** @brief Ajoute un temps au timer
 * @param timer Le timer à mettre à jour
 * @param time Le temps à ajouter
 */
void timer_add_time(Timer *timer, int time);

/** @brief Récupère le temps actuel du timer
 * @param timer Le timer à interroger
 * @return Le temps actuel du timer
 */
int timer_current_time(Timer *timer);

/** @brief Vérifie si le timer a atteint sa limite
 * @param timer Le timer à interroger
 * @param max La limite du timer
 * @return true si le timer a atteint sa limite, false sinon
 */
bool timer_loop(Timer *timer, int max);

/** @brief Crée un nouveau timer
 * @return Le timer créé
 */
Timer timer_create();