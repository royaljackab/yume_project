#include "../lib/components/sprite.h"
#define MIN_LAYER 0
#define MAX_LAYER 100


void drawAll(SpriteManager * spriteManager, CommunManager * communManager){
    
    Vector2 pos;
    int lookup;
    Sprite sprite;
    for (int layer = MIN_LAYER; layer <= MAX_LAYER; layer++){
        for (int i=0; i < spriteManager->count; i++)
        {
            sprite = spriteManager->dense[i];
            lookup = spriteManager->entity_lookup[i];
            if (sprite.renderPriority == layer){
                pos = communManager->dense[lookup];
                if (sprite.display)         
                   DrawSprite(sprite, pos);
            }
        }
    }
}
