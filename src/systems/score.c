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
    scoreSystem->combo = 0;
    scoreSystem->isComboActive = 1;
    scoreSystem->angleSpriteCombo = 0;
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

int update_combo(ScoreSystem *scoreSystem){
    scoreSystem->angleSpriteCombo = GetRandomValue(-40, 40);
    if (scoreSystem->isComboActive){
        scoreSystem->combo++;
        return 1;
    }
    scoreSystem->isComboActive = 1;
    scoreSystem->combo = 0;
    return 0;
}

void draw_combo_sprite(ScoreSystem *scoreSystem, int x, int y){
    SpriteID combo_sprite = get_combo_sprite(scoreSystem);

    Position pos = {x, y, scoreSystem->angleSpriteCombo};

    if (combo_sprite != NULL_SPRITE){
        Sprite_draw_sprite(&sprites[combo_sprite], &pos, NULL);
    }
}

SpriteID get_combo_sprite(ScoreSystem *scoreSystem){
    if (scoreSystem->combo >= 5){
        return COMBO_5;
    }
    if (scoreSystem->combo == 4){
        return COMBO_4;
    }
    if (scoreSystem->combo == 3){
        return COMBO_3;
    }
    if (scoreSystem->combo == 2){
        return COMBO_2;
    }
    if (scoreSystem->combo == 1){
        return COMBO_1;
    }
    return NULL_SPRITE;
}


