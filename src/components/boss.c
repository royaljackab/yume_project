#include "assets.h"
#include "components/common.h"
#include "components/enemy.h"
#include "components/boss.h"

#include "life.h"
#include "obj.h"
#include "sprite.h"
#include "tasks.h"
#include "common_task.h"

#include "pool.h"
#include "screen.h"

#include "raylib.h"
#include <raymath.h>

void bossbar_draw(Life * life){
    DrawRectangle(PANEL_LEFT + 20, 45, PANEL_WIDTH - 40, 20, LIGHTGRAY);
    DrawRectangle(PANEL_LEFT + 30, 50, (PANEL_WIDTH - 60) * ((float)life->life/life->maxLife), 10, RED);
}

void boss_draw_indicator(Pool *p, Entity boss) {
    Sprite indicator = sprites[BOSS_INDICATOR];

    indicator.color.a = 120;
    indicator.scale.x = 2;
    indicator.scale.y = 2;

    Position pos = {{obj_GetX(p, boss), (PANEL_DOWN + SCREEN_HEIGHT) / 2.0 + 5}, 0};
    Tag tag = ENT_PARTICLE;

    Sprite_draw_sprite(&indicator, &pos, &tag);
}

void bossbar_draw_all(Pool * p) {
    for (int i=0; i < p->tag.count; ++i) {
        Tag tag = p->tag.dense[i];
        Entity e = Tag_get_entity(&p->tag, i);

        if (tag == ENT_BOSS) {
            bossbar_draw(Life_get(&p->life, e));
            boss_draw_indicator(p, e);
        }
    }
}

Entity Boss_spawn(Pool *pool, float x, float y, int life, float hitboxRadius, int score, SpriteID graphic) {
    Entity boss = Enemy_spawn(pool, x, y, 0, 0, life, hitboxRadius, score, graphic);
    obj_SetTag(pool, boss, ENT_BOSS);
    obj_AddFlag(pool, boss, FLAG_INVINCIBLE);
    
    INVOKE_SUBTASK(boss_particle_spawner, pool, boss);

    return boss;
}

void Boss_fight_begin(Pool *pool, Entity boss, Vector2 *lens_center, float *lens_radius, float *lens_strength){
    INVOKE_SUBTASK(boss_pentagram_effect, pool, boss);
    INVOKE_SUBTASK(boss_distortion_effect, pool, boss, lens_center, lens_radius, lens_strength);
    INVOKE_SUBTASK(boss_orb_effect, pool, boss);

    obj_RemoveFlag(pool, boss, FLAG_INVINCIBLE);
}