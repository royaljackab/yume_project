#pragma once

#include <stdbool.h>
#include <stdlib.h>

#define MAX_TIMES 100

typedef struct{
    int chrono;
    int nbTime;
    int time[MAX_TIMES];
} Timer;


void timer_add_time(Timer *timer, int time);
int timer_current_time(Timer *timer);
bool timer_loop(Timer *timer, int max);