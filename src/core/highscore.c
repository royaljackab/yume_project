#include <stdio.h>
#include <string.h>
#include "systems/score.h"
#include "core/highscore.h"
#include <raylib.h>

#define FILENAME "../highscore.txt"
#define PREFIX "highscore="

int update_highscore(unsigned int score) {
/** @brief Change le highscore si le score actuel est supérieur au highscore enregistré. Retourne 1 si le highscore a été mis à jour, sinon 0.
 * @param score Le score à vérifier / enregistrer
 * */

    unsigned int currentHighscore = get_highscore();
    if (score >= currentHighscore) {
        return save_highscore(score);
    }
    return 0;
}


int save_highscore(unsigned int score) {
/** @brief Enregistre le score dans un fichier texte. Retourne 1 si l'enregistrement a réussi, sinon 0. 
* @param score Le score à enregistrer
*/ 
    FILE *file = fopen(FILENAME, "w");
    if (!file) {
        return 0;
    }
    fprintf(file, "%s%u\n", PREFIX, score);
    fclose(file);
    return 1;
}


unsigned int get_highscore() {
/** @brief Récupère le score highscore depuis un fichier texte.  
* @return Retourne 0 si le fichier n'existe pas ou en cas d'erreur (réinitialisation/initialisation du score).
*/ 
  FILE *file = fopen(FILENAME, "r");
  unsigned int highscore = 0;
  if (!file) {
    return highscore;
  }
  char prefix[20];
  fscanf(file, "%29[^=]=%d", prefix, &highscore);
  fclose(file);
  
  char highscore_text[64];
  sprintf(highscore_text, "%u", highscore);
  return highscore;

}