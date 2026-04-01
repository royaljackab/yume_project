#include "systems/score.h"
#include "pool.h"

extern void score_system_init(ScoreSystem *scoreSystem){
    scoreSystem->score = 0;
    scoreSystem->graze = 0;
    scoreSystem->scoreOnHit = 100;
    scoreSystem->scoreOnGraze = 10;
    scoreSystem->maxScore = 999999999;
}

extern void score_increase(ScoreSystem *scoreSystem, unsigned int amount){
    scoreSystem->score += amount;
    if(scoreSystem->score > scoreSystem->maxScore){
        scoreSystem->score = scoreSystem->maxScore;
    }
}

