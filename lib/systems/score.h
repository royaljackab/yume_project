#pragma once
typedef struct ScoreSystem {
  unsigned int score;
  unsigned int graze;
  unsigned int scoreOnGraze;
  unsigned int scoreOnHit;
  unsigned int maxScore;
} ScoreSystem;

extern void score_system_init(ScoreSystem *scoreSystem);
extern void score_increase(ScoreSystem *scoreSystem, unsigned int amount);

