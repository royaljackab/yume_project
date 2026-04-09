#pragma once
typedef struct ScoreSystem {
  unsigned int score;
  unsigned int graze;
  unsigned int scoreOnGraze;
  unsigned int scoreOnHit;
  unsigned int maxScore;
  unsigned int last_high_score;
} ScoreSystem;

extern void score_system_init(ScoreSystem *scoreSystem);
extern void score_increase(ScoreSystem *scoreSystem, unsigned int amount);
unsigned int score_system_get_highscore(ScoreSystem *score);
int score_system_is_new_highscore(ScoreSystem *score);