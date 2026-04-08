#include "background.h"
#include "common.h"
#include "pool.h"
#include "screen.h"
#include "sprite.h"

Entity Background_create(Pool *p, SpriteID bgId, float scroll_speed_x, float scroll_speed_y) {
    Entity e = pool_create_entity(p);
    Background bg = { 
        .scroll_speed_x = scroll_speed_x,
        .scroll_speed_y = scroll_speed_y
    };
    Position pos = {
        .pos = {PANEL_LEFT + PANEL_WIDTH/2, PANEL_UP + PANEL_HEIGHT/2},
        .angle = 0
    };
    
    Background_add(&p->background, e, bg);
    Sprite_add(&p->sprite, e, sprites[bgId]);
    Position_add(&p->position, e, pos);

    return e;
}

void Background_update_all(Pool *p) {
    for (int i=0; i < p->background.count; ++i) {
        Entity e = Background_get_entity(&p->background, i);
        Background *bg = &p->background.dense[i];
        Sprite *sprite = Sprite_get(&p->sprite, e);

        if (sprite) {
            sprite->srcRect.x += bg->scroll_speed_x;
            sprite->srcRect.y += bg->scroll_speed_y;
        }
    }
}

