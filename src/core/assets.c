#include "assets.h"

Texture2D textures[MAX_TEXTURES];

void AssetsLoad() {
    textures[BULLET_SPRITESHEET] = LoadTexture("../Assets/Sprites/bullet_spritesheet.png");
    textures[BOSS_SPRITESHEET] = LoadTexture("../Assets/Sprites/boss_spritesheet.png");
    textures[ANIM_TEST_SPRITESHEET] = LoadTexture("../Assets/Sprites/anim_test.png");
    textures[REIMU_SPRITESHEET] = LoadTexture("../Assets/Sprites/reimu_spritesheet.png");
    textures[HITBOX_SPRITESHEET] = LoadTexture("../Assets/Sprites/hitbox.png");
}

void AssetsUnload() {
    for(int i=0; i<MAX_TEXTURES; i++) UnloadTexture(textures[i]);
}