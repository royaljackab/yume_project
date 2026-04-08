#include "components/life.h"
#include "systems/score.h"
#include "components/common.h"
#include "pool.h"

void Life_damage(Life *life, int damage) { life->life -= damage; }

void Life_heal(Life *life, int heal) { life->life += heal; }

bool Life_is_dead(Life *life) { return life->life <= 0; }

void Life_update_all(Pool * p, ScoreSystem * scoreS){
    for(int i = 0; i < p->life.count; i++){
        Entity e = Life_get_entity(&p->life, i);
        Tag *tag = Tag_get(&p->tag, e);
        Life *life = &p->life.dense[i];
        if(Life_is_dead(life)){
            switch(*tag){
                case ENT_ENEMY:
                    PlaySound(sfx[SFX_ENEMY_DEATH]);
                    pool_kill_entity(p, e);
                    score_increase(scoreS, Enemy_get(&p->enemy, e)->score);
                    break;
                case ENT_ENEMY_SHOT:
                    pool_kill_entity(p, e);
                    break;
                case ENT_BOSS:
                case ENT_PLAYER:
                    break;
                default:
                    pool_kill_entity(p,e);
            }
        }
    }
}