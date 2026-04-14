#pragma once
#include "content/assets.h"

typedef struct ScoreSystem {
  unsigned int score;
  unsigned int graze;
  unsigned int scoreOnGraze;
  unsigned int scoreOnHit;
  unsigned int maxScore;
  unsigned int last_high_score;

  unsigned int combo;
  unsigned int isComboActive; //est aussi utilisé en tant que timer pour l'animation de l'indicateur de combo. 0 = combo cassé, 1 = combo actif mais pas d'affichage, >1 = combo actif et affichage
  float angleSpriteCombo;

} ScoreSystem;

extern void score_system_init(ScoreSystem *scoreSystem);
extern void score_increase(ScoreSystem *scoreSystem, unsigned int amount);
unsigned int score_system_get_highscore(ScoreSystem *score);
int score_system_is_new_highscore(ScoreSystem *score);
int update_combo(ScoreSystem *scoreSystem);
SpriteID get_combo_sprite(ScoreSystem *scoreSystem);
void draw_combo_sprite(ScoreSystem *scoreSystem, int x, int y);
extern void score_decrease(ScoreSystem *scoreSystem, unsigned int amount);

