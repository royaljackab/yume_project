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

/** @brief Initialise le système de score
 * @param scoreSystem Le système de score à initialiser
 */
extern void score_system_init(ScoreSystem *scoreSystem);

/** @brief Augmente le score du joueur
 * @param scoreSystem Le système de score à mettre à jour
 * @param amount Le montant à ajouter au score
 */
extern void score_increase(ScoreSystem *scoreSystem, unsigned int amount);

/** @brief Récupère le score actuel du joueur
 * @param scoreSystem Le système de score à interroger
 * @return Le score actuel du joueur
 */
unsigned int score_system_get_highscore(ScoreSystem *score);

/** @brief Vérifie si le score actuel est un nouveau record
 * @param scoreSystem Le système de score à interroger
 * @return 1 si c'est un nouveau record, 0 sinon
 */
int score_system_is_new_highscore(ScoreSystem *score);

/** @brief Met à jour le combo du joueur
 * @param scoreSystem Le système de score à mettre à jour
 * @return 1 si le combo est actif, 0 sinon
 */
int update_combo(ScoreSystem *scoreSystem);

/** @brief Récupère le sprite du combo
 * @param scoreSystem Le système de score à interroger
 * @return L'ID du sprite du combo
 */
SpriteID get_combo_sprite(ScoreSystem *scoreSystem);

/** @brief Dessine le sprite du combo
 * @param scoreSystem Le système de score à interroger
 * @param x La position X où dessiner le sprite
 * @param y La position Y où dessiner le sprite
 */
void draw_combo_sprite(ScoreSystem *scoreSystem, int x, int y);

/** @brief Diminue le score du joueur
 * @param scoreSystem Le système de score à mettre à jour
 * @param amount Le montant à soustraire du score
 */
extern void score_decrease(ScoreSystem *scoreSystem, unsigned int amount);

