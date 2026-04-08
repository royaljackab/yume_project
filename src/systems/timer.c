#include "timer.h"

// Timer timer_create() {
//     /***
//      * créé un nouveau timer;
//      */
//     Timer timer;
//     timer.chrono = 0;
//     timer.nbTime = 0;
//     return tim cer;
// }

bool timer_loop(Timer *timer, int max) {
    /***
     * Remet un timer à 0 si il a atteint sa valeur Max.
     */
    if(timer->chrono >= max) {
        timer->chrono = 0;
        return true;
    }
    return false;
}


void timer_add_time(Timer *timer, int time) {
    /***
     * Ajoute un temps à la liste des temps d'un timer
     */
    if(timer->nbTime < MAX_TIMES) {
        timer->time[timer->nbTime] = time;
        timer->nbTime++;
    }

    //tri des temps
    for (int i = 0; i < timer->nbTime - 1; i++) {
        for (int j = 0; j < timer->nbTime - i - 1; j++) {
            if (timer->time[j] > timer->time[j + 1]) {
                int temp = timer->time[j];
                timer->time[j] = timer->time[j + 1];
                timer->time[j + 1] = temp;
            }
        }
    }
}


bool timer_over_time(Timer *timer, int time){
    return timer_current_time(timer) >= time;
}


int timer_current_time(Timer *timer) {
    /***
     * Retourne l'index du temps actuel du timer
     */
    for(int i = 0; i < timer->nbTime; i++) {
        if(i < timer->nbTime - 1){
            if(timer->chrono >= timer->time[i] && timer->chrono < timer->time[i+1]) {
                return i;
            }
        }
        else if(timer->chrono >= timer->time[i]) {
                return i;
            }           

    }
    return -1;
}





