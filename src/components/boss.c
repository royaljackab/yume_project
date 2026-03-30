#include "components/common.h"
#include "components/enemy.h"
#include "components/boss.h"

#include "pool.h"
#include "screen.h"

#include "raylib.h"
#include <stdio.h>
void bossbar_draw(Life * life){
    DrawRectangle(PANEL_LEFT + 20, 45, PANEL_WIDTH - 40, 20, LIGHTGRAY);
    DrawRectangle(PANEL_LEFT + 30, 50, (PANEL_WIDTH - 60) * ((float)life->life/life->maxLife), 10, RED);
}

void bossbar_draw_all(Pool * p) {
    for(int i = 0; i < p->enemy.count; i++){
        Entity id = p->enemy.entity_lookup[i];
        if (*Tag_get(&p->tag, id) == ENT_BOSS){
            bossbar_draw(Life_get(&p->life,id));
            break; //un seul boss à l'écran à la fois
        }
    }
}