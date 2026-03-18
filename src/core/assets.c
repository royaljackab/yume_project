/**
 * @file assets.c
 * @author jack abribat
 * description blah blah
 */

#include "core/assets.h"
#include "sprite.h"
#include <raylib.h>

Sprite sprites[MAX_SPRITES];
Texture2D textures[MAX_TEXTURES];
Music playlist[MAX_BGM]; // Nouveau tableau pour la musique
Sound sfx[MAX_SFX]; // Effets sonores

void AssetsLoad() {
    // Chargement des Textures existantes
    textures[BULLET_SPRITESHEET] =
      LoadTexture("../Assets/Sprites/bullet_spritesheet.png");
    textures[BOSS_SPRITESHEET] =
      LoadTexture("../Assets/Sprites/boss_spritesheet.png");
    textures[ANIM_TEST_SPRITESHEET] =
      LoadTexture("../Assets/Sprites/anim_test.png");
    textures[REIMU_SPRITESHEET] =
      LoadTexture("../Assets/Sprites/reimu_spritesheet.png");
    textures[HITBOX_SPRITESHEET] = LoadTexture("../Assets/Sprites/hitbox.png");

    // Chargement de la Musique
    playlist[BGM_FAST_DANGER] =
      LoadMusicStream("../Assets/Audio/BGM/fast-danger.wav");
    playlist[BGM_QUIRKY_GOOFY] =
      LoadMusicStream("../Assets/Audio/BGM/quirky-goofy.wav");
    playlist[BGM_TO_BE_NAMED] =
      LoadMusicStream("../Assets/Audio/BGM/tobenamedlater.wav");
    playlist[BGM_WAITING] =
      LoadMusicStream("../Assets/Audio/BGM/waiting-settingstype.wav");

    // Chargement des effets sonores
    sfx[SFX_SHOOT] = LoadSound("../Assets/Audio/SFX/shoot.wav");
    sfx[SFX_ENEMY_DEATH] = LoadSound("../Assets/Audio/SFX/enemy_death.wav");
    sfx[SFX_MENU_NAV] = LoadSound("../Assets/Audio/SFX/menu_nav.wav");
}

void SpritesLoad() {
    Sprite_set_texture(&sprites[BALL_M_BLACK], 0, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_M_BLACK], 5, 54, 16, 16);

    Sprite_set_texture(&sprites[ANIM_TEST], 0, ANIM_TEST_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ANIM_TEST], 0,0,256,16);
    Sprite_set_animation(&sprites[ANIM_TEST], 16, 2);

    Sprite_set_texture(&sprites[REIMU_IDLE], 0, REIMU_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[REIMU_IDLE], 0, 0, 256, 48);
    Sprite_set_animation(&sprites[REIMU_IDLE], 8, 5);

    Sprite_set_texture(&sprites[REIMU_LEFT], 0, REIMU_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[REIMU_LEFT], 0, 48, 256, 48);
    Sprite_set_animation(&sprites[REIMU_LEFT], 8, 5);

    Sprite_set_texture(&sprites[REIMU_RIGHT], 0, REIMU_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[REIMU_RIGHT], 0, 96, 256, 48);
    Sprite_set_animation(&sprites[REIMU_RIGHT], 8, 5);

    Sprite_set_texture(&sprites[REIMU_PINK_AMULET], 0, REIMU_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[REIMU_PINK_AMULET], 138, 178, 56, 14);
    Sprite_set_center(&sprites[REIMU_PINK_AMULET], (Vector2){46,6});
    Sprite_set_rotation(&sprites[REIMU_PINK_AMULET], -90);
}

void AssetsUnload() {
    // Déchargement des textures
  for (int i = 0; i < MAX_TEXTURES; i++)
    UnloadTexture(textures[i]);

    // Déchargement des musiques (Libère la RAM de ton Mac/PC ok)
  for (int i = 0; i < MAX_BGM; i++)
    UnloadMusicStream(playlist[i]);

    // Déchargement des effets sonores
  for (int i = 0; i < MAX_SFX; i++)
    UnloadSound(sfx[i]);
}
