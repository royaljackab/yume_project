#include "systems/score.h"
#include "pool.h"
#include "game_state.h"

#define POINTS_PER_COMBO 4000


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
/** @brief Augmente le score du système de score
 * @param scoreSystem Le système de score
 * @param amount La quantité à ajouter au score
 */
    scoreSystem->score += amount;
    if(scoreSystem->score > scoreSystem->maxScore){
        scoreSystem->score = scoreSystem->maxScore;
    }
}

unsigned int score_system_get_highscore(ScoreSystem *score){
/** @brief Récupère le highscore du système de score (prend en compte le cas ou le score > last_high_score)
 * @param score Le système de score
 * @return Le highscore calculé
 */
    if (score->last_high_score > score->score){
        return score->last_high_score;
    }
    return score->score;
}

int score_system_is_new_highscore(ScoreSystem *score){
/** @brief Vérifie si le score actuel est un nouveau highscore (score > last_high_score)
 * @param score Le système de score
 * @return 1 si c'est un nouveau highscore, 0 sinon
 */
    if (score->last_high_score <= score->score){
        return 1;
    }
    return 0;
}

int update_combo(ScoreSystem *scoreSystem){
/** @brief Met à jour le combo du système de score. Si le combo est actif, incrémente le combo et retourne 1. Sinon, active le combo, réinitialise le combo à 0 et retourne 0. Octroie les points du combo
 * @param scoreSystem Le système de score
 */
    
    scoreSystem->angleSpriteCombo = GetRandomValue(-40, 40);
    if (scoreSystem->isComboActive){
        scoreSystem->combo++;
        score_increase(scoreSystem, POINTS_PER_COMBO * scoreSystem->combo * log(scoreSystem->combo));
        return 1;
    }
    scoreSystem->isComboActive = 1;
    scoreSystem->combo = 0;
    return 0;
}

void draw_combo_sprite(ScoreSystem *scoreSystem, int x, int y){
/** @brief Dessine le sprite de combo en le récuperant via get_combo_sprite(scoreSystem)
 * @param scoreSystem Le système de score
 * @param x La position x
 * @param y La position y
 */
    SpriteID combo_sprite = get_combo_sprite(scoreSystem);

    Position pos = {x, y, scoreSystem->angleSpriteCombo};

    if (combo_sprite != NULL_SPRITE){
        Sprite_draw_sprite(&sprites[combo_sprite], &pos, NULL);
    }
}

SpriteID get_combo_sprite(ScoreSystem *scoreSystem){
/** @brief Récupère le sprite de combo en fonction du niveau de combo dans le scoreSystem
 * @param scoreSystem Le système de score
 * @return L'ID du sprite de combo correspondant
 */
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


extern void score_decrease(ScoreSystem *scoreSystem, unsigned int amount){
    //enleve amount du score du joueur sans le descendre sous 0. (amount est positif)
    scoreSystem->score -= amount;
    if(scoreSystem->score < 0){ //min score = 0
        scoreSystem->score = 0;
    }
}
