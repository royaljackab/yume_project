#include "systems/score.h"
#include "pool.h"
#include "game_state.h"

extern void score_system_init(ScoreSystem *scoreSystem){
    scoreSystem->score = 0;
    scoreSystem->graze = 0;
    scoreSystem->scoreOnHit = 100;
    scoreSystem->scoreOnGraze = 10;
    scoreSystem->maxScore = 999999999;
    scoreSystem->last_high_score = get_highscore();
}

extern void score_increase(ScoreSystem *scoreSystem, unsigned int amount){
    scoreSystem->score += amount;
    if(scoreSystem->score > scoreSystem->maxScore){
        scoreSystem->score = scoreSystem->maxScore;
    }
}

unsigned int score_system_get_highscore(ScoreSystem *score){
    if (score->last_high_score > score->score){
        return score->last_high_score;
    }
    return score->score;
}

int score_system_is_new_highscore(ScoreSystem *score){
    if (score->last_high_score <= score->score){
        return 1;
    }
    return 0;
}

