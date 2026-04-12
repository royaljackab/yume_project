#include "systems/score.h"
#include "pool.h"
#include "game_state.h"
#include "highscore.h"

#define POINTS_PER_COMBO 35000
#define COMBO_ACTIVE_VALUE 80

extern void score_system_init(ScoreSystem *scoreSystem){
    scoreSystem->score = 0;
    scoreSystem->graze = 0;
    scoreSystem->scoreOnHit = 500;
    scoreSystem->scoreOnGraze = 50;
    scoreSystem->maxScore = 999999999;
    scoreSystem->last_high_score = get_highscore();
    scoreSystem->combo = 0;
    scoreSystem->isComboActive = COMBO_ACTIVE_VALUE;
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
extern void score_decrease(ScoreSystem *scoreSystem, unsigned int amount){
/** @brief enleve amount du score du joueur sans le descendre sous 0. (amount est positif)
 * @param scoreSystem Le système de score
 * @param amount La quantité à ajouter au score
 */
    scoreSystem->score -= amount;
    if(scoreSystem->score < 0){ //min score = 0
        scoreSystem->score = 0;
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
    
    scoreSystem->angleSpriteCombo = GetRandomValue(-10, 5); //donne un angle aléatoire au sprite de combo pour le rendre plus dynamique
    if (scoreSystem->isComboActive){
        scoreSystem->combo++;
        scoreSystem->isComboActive = COMBO_ACTIVE_VALUE;
        score_increase(scoreSystem, POINTS_PER_COMBO * scoreSystem->combo * log(scoreSystem->combo));
        return 1;
    }
    scoreSystem->isComboActive = COMBO_ACTIVE_VALUE;
    scoreSystem->combo = 0;
    return 0;
}

#include <stdio.h>
#include <math.h>
void draw_combo_sprite(ScoreSystem *scoreSystem, int x, int y){
/** @brief Dessine le sprite de combo en le récuperant via get_combo_sprite(scoreSystem)
 * @param scoreSystem Le système de score
 * @param x La position x
 * @param y La position y
 */
    SpriteID combo_sprite = get_combo_sprite(scoreSystem);
    
    if (combo_sprite != NULL_SPRITE){
        
        Sprite *sprite = &sprites[combo_sprite];
        Position pos = {x, y, scoreSystem->angleSpriteCombo};
        scoreSystem->angleSpriteCombo += 0.01; //fait tourner le sprite de combo;
        //float val = (4/ pow(COMBO_ACTIVE_VALUE,2)) * scoreSystem->isComboActive * (COMBO_ACTIVE_VALUE - scoreSystem->isComboActive);
        //float val = pow(scoreSystem->isComboActive/(COMBO_ACTIVE_VALUE/2), a) * (1 - pow(scoreSystem->isComboActive/(COMBO_ACTIVE_VALUE), a));
        int a = 5;
        float val = 1.2 * pow(sinf((PI * scoreSystem->isComboActive) / COMBO_ACTIVE_VALUE), a);
        
        float scale = val + (scoreSystem->combo * 0.1);
        sprite->scale.x = scale;
        sprite->scale.y = scale;
        scoreSystem->isComboActive = Clamp(scoreSystem->isComboActive - 1, 0, COMBO_ACTIVE_VALUE); //fait décroître le timer du combo
        Sprite_draw_sprite(sprite, &pos, NULL);
        UpdateAnimation(sprite);
        
    }
    
}

SpriteID get_combo_sprite(ScoreSystem *scoreSystem){
/** @brief Récupère le sprite de combo en fonction du niveau de combo dans le scoreSystem
 * @param scoreSystem Le système de score
 * @return L'ID du sprite de combo correspondant
 */
    if (scoreSystem->isComboActive == 0 || scoreSystem->isComboActive == 1){
        return NULL_SPRITE;
    }
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


