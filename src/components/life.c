#include "components/life.h"
#include "assets.h"
#include "obj.h"
#include "player.h"
#include "systems/score.h"
#include "components/common.h"
#include "common_task.h"
#include "pool.h"
#include "tasks.h"

void Life_damage(Life *life, int damage) { life->life -= damage; }

void Life_heal(Life *life, int heal) { life->life += heal; }

bool Life_is_dead(Life *life) { return life->life <= 0; }

void Life_update_all(GameContext *ctx, ScoreSystem * scoreS){
    Pool *p = ctx->pool;

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
                    float shot_x = obj_GetX(p, e);
                    float shot_y = obj_GetY(p, e);
                    SCHED_INVOKE_TASK(&ctx->sched, play_anim_once, p, shot_x, shot_y, 4, 4, BULLET_DESTROY, 255);
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