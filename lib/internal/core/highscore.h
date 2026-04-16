
/** @brief Récupère le score maximum atteint en lisant dans le fichier highscore.txt 
 * @return Le score maximum atteint
 */
unsigned int get_highscore();

/** @brief Enregistre le score dans le fichier highscore.txt 
 * @param score Le score à enregistrer
 * @return 1 si l'enregistrement a réussi, 0 sinon
 */
int save_highscore(unsigned int score);

/** @brief Met à jour le highscore si le score actuel est supérieur au highscore enregistré. Retourne 1 si le highscore a été mis à jour, sinon 0.
 * @param score Le score à vérifier / enregistrer
 */
int update_highscore(unsigned int score);