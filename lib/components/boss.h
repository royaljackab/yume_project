#include "components/life.h"
#include "cotask.h"
#include "obj.h"
#include "pool.h"
#include "tasks.h"

void bossbar_draw(Life * life);
void bossbar_draw_all(Pool * p);

/**
 * @brief Crée une entité boss, affiche ses particules visuelles passives.
 * 
 * @param pool 
 * @param x 
 * @param y 
 * @param life 
 * @param hitboxRadius 
 * @param score 
 * @param graphic 
 * @return Entity : Id du boss
 */
Entity Boss_spawn(Pool *pool, float x, float y, int life, float hitboxRadius, int score, SpriteID graphic);

void Boss_fight_begin(Pool *pool, Entity boss, Vector2 *lens_center, float *lens_radius, float *lens_strength);