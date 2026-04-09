/**
 * @file assets.c
 * @author jack abribat
 * description blah blah
 */

#include "content/assets.h"
#include "sprite.h"
#include "pool.h"
#include "obj.h"
#include "screen.h"
#include <raylib.h>

Sprite sprites[MAX_SPRITES];
Texture2D textures[MAX_TEXTURES];
Music playlist[MAX_BGM]; // Nouveau tableau pour la musique
Sound sfx[MAX_SFX]; // Effets sonores
Font fonts[MAX_FONTS]; // Polices d'écritures

extern Entity invoke_main_background(Pool *p) {
    Entity base = Background_create(p, BG_MAIN, 0, 0);

    obj_SetScaleX(p, base, (float)SCREEN_WIDTH/(float)sprites[BG_MAIN].srcRect.width);
    obj_SetScaleY(p, base, (float)SCREEN_HEIGHT/(float)sprites[BG_MAIN].srcRect.height);
    
    return base;
}

void FontsLoad(){
  /**
   * @brief 
   */
  fonts[TOUHOU_98] = LoadFont("../Assets/Fonts/touhou98.ttf");
}

void AssetsLoad() {
    // Chargement des Textures existantes
    textures[BULLET_SPRITESHEET] =
      LoadTexture("../Assets/Sprites/bullet_spritesheet.png");
  textures[ENEMY_SPRITESHEET] =
      LoadTexture("../Assets/Sprites/enemies.png");
    textures[BOSS_SPRITESHEET] =
      LoadTexture("../Assets/Sprites/boss_spritesheet.png");
    textures[ANIM_TEST_SPRITESHEET] =
      LoadTexture("../Assets/Sprites/anim_test.png");
    textures[REIMU_SPRITESHEET] =
      LoadTexture("../Assets/Sprites/reimu_spritesheet.png");
  textures[HITBOX_SPRITESHEET] = 
      LoadTexture("../Assets/Sprites/hitbox.png");


    // Backgrounds
    textures[BG_SC_FLOWERS] = LoadTexture("../Assets/Sprites/bg/bg_touhou_flowers.png");
    textures[BG_SC_OV_CIRCLES] = LoadTexture("../Assets/Sprites/bg/bg_touhou_gray_circles.png");
    textures[BG_SC_MAIN] = LoadTexture("../Assets/Sprites/bg/bg_menu.png");
    
    SetTextureWrap(textures[BG_SC_OV_CIRCLES], TEXTURE_WRAP_REPEAT);


    // Chargement de la Musique
    playlist[BGM_FAST_DANGER] =
      LoadMusicStream("../Assets/Audio/BGM/fast-danger.wav");
    playlist[BGM_QUIRKY_GOOFY] =
      LoadMusicStream("../Assets/Audio/BGM/quirky-goofy.wav");
    playlist[BGM_TO_BE_NAMED] =
      LoadMusicStream("../Assets/Audio/BGM/tobenamedlater.wav");
    playlist[BGM_WAITING] =
      LoadMusicStream("../Assets/Audio/BGM/waiting-settingstype.wav");
    playlist[BGM_MORIYA_THEME] =
      LoadMusicStream("../Assets/Audio/BGM/moriya_theme.wav");

    // Chargement des effets sonores
    sfx[SFX_SHOOT] = LoadSound("../Assets/Audio/SFX/shoot.wav");
    sfx[SFX_ENEMY_DEATH] = LoadSound("../Assets/Audio/SFX/enemy_death.wav");
    sfx[SFX_MENU_NAV] = LoadSound("../Assets/Audio/SFX/menu_nav.wav");
    sfx[SFX_TAN00] = LoadSound("../Assets/Audio/SFX/touhou_sfx/se_tan00.wav");
    sfx[SFX_TAN01] = LoadSound("../Assets/Audio/SFX/touhou_sfx/se_tan01.wav");
    sfx[SFX_TAN02] = LoadSound("../Assets/Audio/SFX/touhou_sfx/se_tan02.wav");
    sfx[SFX_PLDEAD00] = LoadSound("../Assets/Audio/SFX/touhou_sfx/se_pldead00.wav");

    sfx[SFX_LAZER00] = LoadSound("../Assets/Audio/SFX/touhou_sfx/se_lazer00.wav");
    sfx[SFX_LAZER01] = LoadSound("../Assets/Audio/SFX/touhou_sfx/se_lazer01.wav");
    sfx[SFX_LAZER02] = LoadSound("../Assets/Audio/SFX/touhou_sfx/se_lazer02.wav");

    sfx[SFX_GRAZE] = LoadSound("../Assets/Audio/SFX/touhou_sfx/se_graze.wav");

    // Load dem enemies up
    EnemiesSpritesLoad();

  for (int i = 0; i < MAX_TEXTURES; i++) {
        // On vérifie que la texture existe (id > 0) pour éviter les crashs
        if (textures[i].id != 0) { 
            SetTextureFilter(textures[i], TEXTURE_FILTER_BILINEAR);
        }
    }
}

void SpritesLoad() {
    Sprite_set_texture(&sprites[ANIM_TEST], 10, ANIM_TEST_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ANIM_TEST], 0,0,256,16);
    Sprite_set_animation(&sprites[ANIM_TEST], 16, 2);

    Sprite_set_texture(&sprites[REIMU_IDLE], 10, REIMU_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[REIMU_IDLE], 0, 0, 256, 48);
    Sprite_set_animation(&sprites[REIMU_IDLE], 8, 5);

    Sprite_set_texture(&sprites[REIMU_LEFT], 10, REIMU_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[REIMU_LEFT], 0, 48, 256, 48);
    Sprite_set_animation(&sprites[REIMU_LEFT], 8, 5);

    Sprite_set_texture(&sprites[REIMU_RIGHT], 10, REIMU_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[REIMU_RIGHT], 0, 96, 256, 48);
    Sprite_set_animation(&sprites[REIMU_RIGHT], 8, 5);

    Sprite_set_texture(&sprites[REIMU_PINK_AMULET], 10, REIMU_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[REIMU_PINK_AMULET], 138, 178, 56, 14);
    Sprite_set_center(&sprites[REIMU_PINK_AMULET], (Vector2){46,6});
    Sprite_set_rotation(&sprites[REIMU_PINK_AMULET], -90);

    Sprite_set_texture(&sprites[HITBOX], 11, HITBOX_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[HITBOX], 0, 0, 64, 64);
    Sprite_set_rotation(&sprites[HITBOX], 0);


    BulletsSpritesLoad();
    BgSpritesLoad();
    EffectsLoad();
}

void BgSpritesLoad() {
  Sprite_set_texture(&sprites[BG_MORIYA_FLOWERS], RENDER_PRIO_BG, BG_SC_FLOWERS);
  Sprite_set_SourceRect(&sprites[BG_MORIYA_FLOWERS], 0, 0, 384, 448);

  Sprite_set_texture(&sprites[BG_MORIYA_CIRCLES], RENDER_PRIO_BG, BG_SC_OV_CIRCLES);
  Sprite_set_SourceRect(&sprites[BG_MORIYA_CIRCLES], 0, 0, 2000, 2000);

  Sprite_set_texture(&sprites[BG_MAIN], RENDER_PRIO_BG, BG_SC_MAIN);
  Sprite_set_SourceRect(&sprites[BG_MAIN], 0, 0, 3840, 2160);
}

void EffectsLoad() {
  Sprite_set_texture(&sprites[BOSS_AURA_WAVES], 0, BULLET_SPRITESHEET);
  Sprite_set_SourceRect(&sprites[BOSS_AURA_WAVES], 384, 429, 42, 35);
  Sprite_set_center(&sprites[BOSS_AURA_WAVES], (Vector2){0, 17.5});

  Sprite_set_texture(&sprites[BOSS_PENTAGRAM], 0, BULLET_SPRITESHEET);
  Sprite_set_SourceRect(&sprites[BOSS_PENTAGRAM], 613, 400, 128, 128);

  Sprite_set_texture(&sprites[BOSS_AURA_ORB], 0, BULLET_SPRITESHEET);
  Sprite_set_SourceRect(&sprites[BOSS_AURA_ORB], 32, 32, 225, 225);
}

//CLANKER GPT CORE
void BulletsSpritesLoad() {
    /**
     *  Fonction intermediaire pour load tous les assets du jeu.
     *  Load toutes les bullets.
     **/
    Sprite_set_texture(&sprites[NULL_SPRITE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[NULL_SPRITE], 0, 0, 0, 0);


    Sprite_set_texture(&sprites[BALL_M_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_M_BLACK], 0, 17, 16, 16);

    // LASERS
    Sprite_set_texture(&sprites[LASER_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[LASER_BLACK], 807, 0, 14, 16);

    Sprite_set_texture(&sprites[LASER_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[LASER_RED], 807, 16, 14, 16);

    Sprite_set_texture(&sprites[LASER_LIGHT_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[LASER_LIGHT_RED], 807, 32, 14, 16);

    Sprite_set_texture(&sprites[LASER_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[LASER_PINK], 807, 48, 14, 16);

    Sprite_set_texture(&sprites[LASER_LIGHT_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[LASER_LIGHT_PINK], 807, 64, 14, 16);

    Sprite_set_texture(&sprites[LASER_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[LASER_BLUE], 807, 80, 14, 16);

    Sprite_set_texture(&sprites[LASER_LIGHT_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[LASER_LIGHT_BLUE], 807, 96, 14, 16);

    Sprite_set_texture(&sprites[LASER_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[LASER_CYAN], 807, 112, 14, 16);
    
    Sprite_set_texture(&sprites[LASER_LIGHT_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[LASER_LIGHT_CYAN], 807, 128, 14, 16);

    Sprite_set_texture(&sprites[LASER_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[LASER_GREEN], 807, 144, 14, 16);

    Sprite_set_texture(&sprites[LASER_LIGHT_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[LASER_LIGHT_GREEN], 807, 160, 14, 16);

    Sprite_set_texture(&sprites[LASER_LIME], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[LASER_LIME], 807, 176, 14, 16);

    Sprite_set_texture(&sprites[LASER_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[LASER_YELLOW], 807, 192, 14, 16);

    Sprite_set_texture(&sprites[LASER_LIGHT_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[LASER_LIGHT_YELLOW], 807, 208, 14, 16);

    Sprite_set_texture(&sprites[LASER_GOLD], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[LASER_GOLD], 807, 224, 14, 16);

    Sprite_set_texture(&sprites[LASER_WHITE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[LASER_WHITE], 807, 240, 14, 16);

// MISSILES
    Sprite_set_texture(&sprites[MISSILE_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[MISSILE_BLACK], 790, 0, 15, 16);

    Sprite_set_texture(&sprites[MISSILE_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[MISSILE_RED], 790, 16, 15, 16);

    Sprite_set_texture(&sprites[MISSILE_LIGHT_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[MISSILE_LIGHT_RED], 790, 32, 15, 16);

    Sprite_set_texture(&sprites[MISSILE_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[MISSILE_PINK], 790, 48, 15, 16);

    Sprite_set_texture(&sprites[MISSILE_LIGHT_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[MISSILE_LIGHT_PINK], 790, 64, 15, 16);

    Sprite_set_texture(&sprites[MISSILE_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[MISSILE_BLUE], 790, 80, 15, 16);

    Sprite_set_texture(&sprites[MISSILE_LIGHT_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[MISSILE_LIGHT_BLUE], 790, 96, 15, 16);

    Sprite_set_texture(&sprites[MISSILE_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[MISSILE_CYAN], 790, 112, 15, 16);
    
    Sprite_set_texture(&sprites[MISSILE_LIGHT_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[MISSILE_LIGHT_CYAN], 790, 128, 15, 16);

    Sprite_set_texture(&sprites[MISSILE_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[MISSILE_GREEN], 790, 144, 15, 16);

    Sprite_set_texture(&sprites[MISSILE_LIGHT_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[MISSILE_LIGHT_GREEN], 790, 160, 15, 16);

    Sprite_set_texture(&sprites[MISSILE_LIME], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[MISSILE_LIME], 790, 176, 15, 16);

    Sprite_set_texture(&sprites[MISSILE_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[MISSILE_YELLOW], 790, 192, 15, 16);

    Sprite_set_texture(&sprites[MISSILE_LIGHT_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[MISSILE_LIGHT_YELLOW], 790, 208, 15, 16);

    Sprite_set_texture(&sprites[MISSILE_GOLD], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[MISSILE_GOLD], 790, 224, 15, 16);

    Sprite_set_texture(&sprites[MISSILE_WHITE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[MISSILE_WHITE], 790, 240, 15, 16);

    // RING BULLETS
    Sprite_set_texture(&sprites[BULLET_RING_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_BLACK], 774, 0, 16, 16);

    Sprite_set_texture(&sprites[BULLET_RING_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_RED], 774, 16, 16, 16);

    Sprite_set_texture(&sprites[BULLET_RING_LIGHT_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_LIGHT_RED], 774, 32, 16, 16);

    Sprite_set_texture(&sprites[BULLET_RING_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_PINK], 774, 48, 16, 16);

    Sprite_set_texture(&sprites[BULLET_RING_LIGHT_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_LIGHT_PINK], 774, 64, 16, 16);

    Sprite_set_texture(&sprites[BULLET_RING_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_BLUE], 774, 80, 16, 16);

    Sprite_set_texture(&sprites[BULLET_RING_LIGHT_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_LIGHT_BLUE], 774, 96, 16, 16);

    Sprite_set_texture(&sprites[BULLET_RING_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_CYAN], 774, 112, 16, 16);
    
    Sprite_set_texture(&sprites[BULLET_RING_LIGHT_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_LIGHT_CYAN], 774, 128, 16, 16);

    Sprite_set_texture(&sprites[BULLET_RING_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_GREEN], 774, 144, 16, 16);

    Sprite_set_texture(&sprites[BULLET_RING_LIGHT_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_LIGHT_GREEN], 774, 160, 16, 16);

    Sprite_set_texture(&sprites[BULLET_RING_LIME], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_LIME], 774, 176, 16, 16);

    Sprite_set_texture(&sprites[BULLET_RING_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_YELLOW], 774, 192, 16, 16);

    Sprite_set_texture(&sprites[BULLET_RING_LIGHT_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_LIGHT_YELLOW], 774, 208, 16, 16);

    Sprite_set_texture(&sprites[BULLET_RING_GOLD], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_GOLD], 774, 224, 16, 16);

    Sprite_set_texture(&sprites[BULLET_RING_WHITE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_WHITE], 774, 240, 16, 16);

    //BULLETS
    Sprite_set_texture(&sprites[BULLET_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_BLACK], 758, 0, 16, 16);

    Sprite_set_texture(&sprites[BULLET_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RED], 758, 16, 16, 16);

    Sprite_set_texture(&sprites[BULLET_LIGHT_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_LIGHT_RED], 758, 32, 16, 16);

    Sprite_set_texture(&sprites[BULLET_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_PINK], 758, 48, 16, 16);

    Sprite_set_texture(&sprites[BULLET_LIGHT_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_LIGHT_PINK], 758, 64, 16, 16);

    Sprite_set_texture(&sprites[BULLET_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_BLUE], 758, 80, 16, 16);

    Sprite_set_texture(&sprites[BULLET_LIGHT_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_LIGHT_BLUE], 758, 96, 16, 16);

    Sprite_set_texture(&sprites[BULLET_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_CYAN], 758, 112, 16, 16);
    
    Sprite_set_texture(&sprites[BULLET_LIGHT_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_LIGHT_CYAN], 758, 128, 16, 16);

    Sprite_set_texture(&sprites[BULLET_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_GREEN], 758, 144, 16, 16);

    Sprite_set_texture(&sprites[BULLET_LIGHT_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_LIGHT_GREEN], 758, 160, 16, 16);

    Sprite_set_texture(&sprites[BULLET_LIME], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_LIME], 758, 176, 16, 16);

    Sprite_set_texture(&sprites[BULLET_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_YELLOW], 758, 192, 16, 16);

    Sprite_set_texture(&sprites[BULLET_LIGHT_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_LIGHT_YELLOW], 758, 208, 16, 16);

    Sprite_set_texture(&sprites[BULLET_GOLD], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_GOLD], 758, 224, 16, 16);

    Sprite_set_texture(&sprites[BULLET_WHITE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_WHITE], 758, 240, 16, 16);
   
    //PETALS
    Sprite_set_texture(&sprites[PETAL_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[PETAL_BLACK], 743, 4, 14, 8);

    Sprite_set_texture(&sprites[PETAL_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[PETAL_RED], 743, 20, 14, 8);

    Sprite_set_texture(&sprites[PETAL_LIGHT_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[PETAL_LIGHT_RED], 743, 36, 14, 8);

    Sprite_set_texture(&sprites[PETAL_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[PETAL_PINK], 743, 52, 14, 8);

    Sprite_set_texture(&sprites[PETAL_LIGHT_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[PETAL_LIGHT_PINK], 743, 68, 14, 8);

    Sprite_set_texture(&sprites[PETAL_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[PETAL_BLUE], 743, 84, 14, 8);

    Sprite_set_texture(&sprites[PETAL_LIGHT_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[PETAL_LIGHT_BLUE], 743, 100, 14, 8);

    Sprite_set_texture(&sprites[PETAL_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[PETAL_CYAN], 743, 116, 14, 8);
    
    Sprite_set_texture(&sprites[PETAL_LIGHT_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[PETAL_LIGHT_CYAN], 743, 132, 14, 8);

    Sprite_set_texture(&sprites[PETAL_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[PETAL_GREEN], 743, 148, 14, 8);

    Sprite_set_texture(&sprites[PETAL_LIGHT_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[PETAL_LIGHT_GREEN], 743, 164, 14, 8);

    Sprite_set_texture(&sprites[PETAL_LIME], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[PETAL_LIME], 743, 180, 14, 8);

    Sprite_set_texture(&sprites[PETAL_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[PETAL_YELLOW], 743, 196, 14, 8);

    Sprite_set_texture(&sprites[PETAL_LIGHT_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[PETAL_LIGHT_YELLOW], 743, 212, 14, 8);

    Sprite_set_texture(&sprites[PETAL_GOLD], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[PETAL_GOLD], 743, 228, 14, 8);

    Sprite_set_texture(&sprites[PETAL_WHITE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[PETAL_WHITE], 743, 244, 14, 8);

    //KUNAI
    Sprite_set_texture(&sprites[KUNAI_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[KUNAI_BLACK], 726, 4, 16, 8);

    Sprite_set_texture(&sprites[KUNAI_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[KUNAI_RED], 726, 20, 16, 8);

    Sprite_set_texture(&sprites[KUNAI_LIGHT_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[KUNAI_LIGHT_RED], 726, 36, 16, 8);

    Sprite_set_texture(&sprites[KUNAI_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[KUNAI_PINK], 726, 52, 16, 8);

    Sprite_set_texture(&sprites[KUNAI_LIGHT_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[KUNAI_LIGHT_PINK], 726, 68, 16, 8);

    Sprite_set_texture(&sprites[KUNAI_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[KUNAI_BLUE], 726, 84, 16, 8);

    Sprite_set_texture(&sprites[KUNAI_LIGHT_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[KUNAI_LIGHT_BLUE], 726, 100, 16, 8);

    Sprite_set_texture(&sprites[KUNAI_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[KUNAI_CYAN], 726, 116, 16, 8);
    
    Sprite_set_texture(&sprites[KUNAI_LIGHT_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[KUNAI_LIGHT_CYAN], 726, 132, 16, 8);

    Sprite_set_texture(&sprites[KUNAI_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[KUNAI_GREEN], 726, 148, 16, 8);

    Sprite_set_texture(&sprites[KUNAI_LIGHT_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[KUNAI_LIGHT_GREEN], 726, 164, 16, 8);

    Sprite_set_texture(&sprites[KUNAI_LIME], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[KUNAI_LIME], 726, 180, 16, 8);

    Sprite_set_texture(&sprites[KUNAI_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[KUNAI_YELLOW], 726, 196, 16, 8);

    Sprite_set_texture(&sprites[KUNAI_LIGHT_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[KUNAI_LIGHT_YELLOW], 726, 212, 16, 8);

    Sprite_set_texture(&sprites[KUNAI_GOLD], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[KUNAI_GOLD], 726, 228, 16, 8);

    Sprite_set_texture(&sprites[KUNAI_WHITE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[KUNAI_WHITE], 726, 244, 16, 8);

    //SHARDS
    Sprite_set_texture(&sprites[SHARD_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[SHARD_BLACK], 710, 4, 16, 8);

    Sprite_set_texture(&sprites[SHARD_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[SHARD_RED], 710, 20, 16, 8);

    Sprite_set_texture(&sprites[SHARD_LIGHT_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[SHARD_LIGHT_RED], 710, 36, 16, 8);

    Sprite_set_texture(&sprites[SHARD_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[SHARD_PINK], 710, 52, 16, 8);

    Sprite_set_texture(&sprites[SHARD_LIGHT_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[SHARD_LIGHT_PINK], 710, 68, 16, 8);

    Sprite_set_texture(&sprites[SHARD_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[SHARD_BLUE], 710, 84, 16, 8);

    Sprite_set_texture(&sprites[SHARD_LIGHT_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[SHARD_LIGHT_BLUE], 710, 100, 16, 8);

    Sprite_set_texture(&sprites[SHARD_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[SHARD_CYAN], 710, 116, 16, 8);

    Sprite_set_texture(&sprites[SHARD_LIGHT_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[SHARD_LIGHT_CYAN], 710, 132, 16, 8);

    Sprite_set_texture(&sprites[SHARD_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[SHARD_GREEN], 710, 148, 16, 8);

    Sprite_set_texture(&sprites[SHARD_LIGHT_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[SHARD_LIGHT_GREEN], 710, 164, 16, 8);

    Sprite_set_texture(&sprites[SHARD_LIME], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[SHARD_LIME], 710, 180, 16, 8);

    Sprite_set_texture(&sprites[SHARD_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[SHARD_YELLOW], 710, 196, 16, 8);

    Sprite_set_texture(&sprites[SHARD_LIGHT_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[SHARD_LIGHT_YELLOW], 710, 212, 16, 8);

    Sprite_set_texture(&sprites[SHARD_GOLD], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[SHARD_GOLD], 710, 228, 16, 8);

    Sprite_set_texture(&sprites[SHARD_WHITE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[SHARD_WHITE], 710, 244, 16, 8);
    

    //TALISMANS
    Sprite_set_texture(&sprites[TALISMAN_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[TALISMAN_BLACK], 694, 1, 16, 14);

    Sprite_set_texture(&sprites[TALISMAN_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[TALISMAN_RED], 694, 17, 16, 14);

    Sprite_set_texture(&sprites[TALISMAN_LIGHT_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[TALISMAN_LIGHT_RED], 694, 33, 16, 14);

    Sprite_set_texture(&sprites[TALISMAN_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[TALISMAN_PINK], 694, 49, 16, 14);

    Sprite_set_texture(&sprites[TALISMAN_LIGHT_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[TALISMAN_LIGHT_PINK], 694, 65, 16, 14);

    Sprite_set_texture(&sprites[TALISMAN_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[TALISMAN_BLUE], 694, 81, 16, 14);

    Sprite_set_texture(&sprites[TALISMAN_LIGHT_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[TALISMAN_LIGHT_BLUE], 694, 97, 16, 14);

    Sprite_set_texture(&sprites[TALISMAN_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[TALISMAN_CYAN], 694, 113, 16, 14);

    Sprite_set_texture(&sprites[TALISMAN_LIGHT_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[TALISMAN_LIGHT_CYAN], 694, 129, 16, 14);

    Sprite_set_texture(&sprites[TALISMAN_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[TALISMAN_GREEN], 694, 145, 16, 14);

    Sprite_set_texture(&sprites[TALISMAN_LIGHT_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[TALISMAN_LIGHT_GREEN], 694, 161, 16, 14);

    Sprite_set_texture(&sprites[TALISMAN_LIME], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[TALISMAN_LIME], 694, 177, 16, 14);

    Sprite_set_texture(&sprites[TALISMAN_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[TALISMAN_YELLOW], 694, 193, 16, 14);

    Sprite_set_texture(&sprites[TALISMAN_LIGHT_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[TALISMAN_LIGHT_YELLOW], 694, 209, 16, 14);

    Sprite_set_texture(&sprites[TALISMAN_GOLD], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[TALISMAN_GOLD], 694, 225, 16, 14);

    Sprite_set_texture(&sprites[TALISMAN_WHITE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[TALISMAN_WHITE], 694, 241, 16, 14);


    //GUNSHOTS
    Sprite_set_texture(&sprites[GUNSHOT_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[GUNSHOT_BLACK], 678, 4, 16, 8);

    Sprite_set_texture(&sprites[GUNSHOT_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[GUNSHOT_RED], 678, 20, 16, 8);

    Sprite_set_texture(&sprites[GUNSHOT_LIGHT_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[GUNSHOT_LIGHT_RED], 678, 36, 16, 8);

    Sprite_set_texture(&sprites[GUNSHOT_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[GUNSHOT_PINK], 678, 52, 16, 8);

    Sprite_set_texture(&sprites[GUNSHOT_LIGHT_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[GUNSHOT_LIGHT_PINK], 678, 68, 16, 8);

    Sprite_set_texture(&sprites[GUNSHOT_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[GUNSHOT_BLUE], 678, 84, 16, 8);

    Sprite_set_texture(&sprites[GUNSHOT_LIGHT_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[GUNSHOT_LIGHT_BLUE], 678, 100, 16, 8);

    Sprite_set_texture(&sprites[GUNSHOT_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[GUNSHOT_CYAN], 678, 116, 16, 8);

    Sprite_set_texture(&sprites[GUNSHOT_LIGHT_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[GUNSHOT_LIGHT_CYAN], 678, 132, 16, 8);

    Sprite_set_texture(&sprites[GUNSHOT_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[GUNSHOT_GREEN], 678, 148, 16, 8);

    Sprite_set_texture(&sprites[GUNSHOT_LIGHT_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[GUNSHOT_LIGHT_GREEN], 678, 164, 16, 8);

    Sprite_set_texture(&sprites[GUNSHOT_LIME], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[GUNSHOT_LIME], 678, 180, 16, 8);

    Sprite_set_texture(&sprites[GUNSHOT_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[GUNSHOT_YELLOW], 678, 196, 16, 8);

    Sprite_set_texture(&sprites[GUNSHOT_LIGHT_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[GUNSHOT_LIGHT_YELLOW], 678, 212, 16, 8);

    Sprite_set_texture(&sprites[GUNSHOT_GOLD], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[GUNSHOT_GOLD], 678, 228, 16, 8);

    Sprite_set_texture(&sprites[GUNSHOT_WHITE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[GUNSHOT_WHITE], 678, 244, 16, 8);


    //DARK PETALS
    Sprite_set_texture(&sprites[DARK_PETAL_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_PETAL_BLACK], 662, 4, 14, 8);

    Sprite_set_texture(&sprites[DARK_PETAL_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_PETAL_RED], 662, 20, 14, 8);

    Sprite_set_texture(&sprites[DARK_PETAL_LIGHT_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_PETAL_LIGHT_RED], 662, 36, 14, 8);

    Sprite_set_texture(&sprites[DARK_PETAL_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_PETAL_PINK], 662, 52, 14, 8);

    Sprite_set_texture(&sprites[DARK_PETAL_LIGHT_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_PETAL_LIGHT_PINK], 662, 68, 14, 8);

    Sprite_set_texture(&sprites[DARK_PETAL_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_PETAL_BLUE], 662, 84, 14, 8);

    Sprite_set_texture(&sprites[DARK_PETAL_LIGHT_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_PETAL_LIGHT_BLUE], 662, 100, 14, 8);

    Sprite_set_texture(&sprites[DARK_PETAL_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_PETAL_CYAN], 662, 116, 14, 8);

    Sprite_set_texture(&sprites[DARK_PETAL_LIGHT_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_PETAL_LIGHT_CYAN], 662, 132, 14, 8);

    Sprite_set_texture(&sprites[DARK_PETAL_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_PETAL_GREEN], 662, 148, 14, 8);

    Sprite_set_texture(&sprites[DARK_PETAL_LIGHT_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_PETAL_LIGHT_GREEN], 662, 164, 14, 8);

    Sprite_set_texture(&sprites[DARK_PETAL_LIME], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_PETAL_LIME], 662, 180, 14, 8);

    Sprite_set_texture(&sprites[DARK_PETAL_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_PETAL_YELLOW], 662, 196, 14, 8);

    Sprite_set_texture(&sprites[DARK_PETAL_LIGHT_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_PETAL_LIGHT_YELLOW], 662, 212, 14, 8);

    Sprite_set_texture(&sprites[DARK_PETAL_GOLD], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_PETAL_GOLD], 662, 228, 14, 8);

    Sprite_set_texture(&sprites[DARK_PETAL_WHITE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_PETAL_WHITE], 662, 244, 14, 8);

//STARS
    Sprite_set_texture(&sprites[STAR_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[STAR_BLACK], 647, 0, 15, 16);

    Sprite_set_texture(&sprites[STAR_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[STAR_RED], 647, 16, 15, 16);

    Sprite_set_texture(&sprites[STAR_LIGHT_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[STAR_LIGHT_RED], 647, 32, 15, 16);

    Sprite_set_texture(&sprites[STAR_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[STAR_PINK], 647, 48, 15, 16);

    Sprite_set_texture(&sprites[STAR_LIGHT_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[STAR_LIGHT_PINK], 647, 64, 15, 16);

    Sprite_set_texture(&sprites[STAR_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[STAR_BLUE], 647, 80, 15, 16);

    Sprite_set_texture(&sprites[STAR_LIGHT_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[STAR_LIGHT_BLUE], 647, 96, 15, 16);

    Sprite_set_texture(&sprites[STAR_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[STAR_CYAN], 647, 112, 15, 16);

    Sprite_set_texture(&sprites[STAR_LIGHT_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[STAR_LIGHT_CYAN], 647, 128, 15, 16);

    Sprite_set_texture(&sprites[STAR_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[STAR_GREEN], 647, 144, 15, 16);

    Sprite_set_texture(&sprites[STAR_LIGHT_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[STAR_LIGHT_GREEN], 647, 160, 15, 16);

    Sprite_set_texture(&sprites[STAR_LIME], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[STAR_LIME], 647, 176, 15, 16);

    Sprite_set_texture(&sprites[STAR_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[STAR_YELLOW], 647, 192, 15, 16);

    Sprite_set_texture(&sprites[STAR_LIGHT_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[STAR_LIGHT_YELLOW], 647, 208, 15, 16);

    Sprite_set_texture(&sprites[STAR_GOLD], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[STAR_GOLD], 647, 224, 15, 16);

    Sprite_set_texture(&sprites[STAR_WHITE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[STAR_WHITE], 647, 240, 15, 16);

//BALLS
    Sprite_set_texture(&sprites[BALL_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_BLACK], 630, 0, 16, 16);

    Sprite_set_texture(&sprites[BALL_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_RED], 630, 16, 16, 16);

    Sprite_set_texture(&sprites[BALL_LIGHT_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_LIGHT_RED], 630, 32, 16, 16);

    Sprite_set_texture(&sprites[BALL_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_PINK], 630, 48, 16, 16);

    Sprite_set_texture(&sprites[BALL_LIGHT_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_LIGHT_PINK], 630, 64, 16, 16);

    Sprite_set_texture(&sprites[BALL_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_BLUE], 630, 80, 16, 16);

    Sprite_set_texture(&sprites[BALL_LIGHT_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_LIGHT_BLUE], 630, 96, 16, 16);

    Sprite_set_texture(&sprites[BALL_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_CYAN], 630, 112, 16, 16);

    Sprite_set_texture(&sprites[BALL_LIGHT_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_LIGHT_CYAN], 630, 128, 16, 16);

    Sprite_set_texture(&sprites[BALL_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_GREEN], 630, 144, 16, 16);

    Sprite_set_texture(&sprites[BALL_LIGHT_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_LIGHT_GREEN], 630, 160, 16, 16);

    Sprite_set_texture(&sprites[BALL_LIME], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_LIME], 630, 176, 16, 16);

    Sprite_set_texture(&sprites[BALL_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_YELLOW], 630, 192, 16, 16);

    Sprite_set_texture(&sprites[BALL_LIGHT_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_LIGHT_YELLOW], 630, 208, 16, 16);

    Sprite_set_texture(&sprites[BALL_GOLD], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_GOLD], 630, 224, 16, 16);

    Sprite_set_texture(&sprites[BALL_WHITE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_WHITE], 630, 240, 16, 16);
  
    //DARK MINI BULLETS
    Sprite_set_texture(&sprites[DARK_BULLET_MINI_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_BULLET_MINI_BLACK], 622, 0, 8, 8);

    Sprite_set_texture(&sprites[DARK_BULLET_MINI_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_BULLET_MINI_RED], 622, 8, 8, 8);
  
    Sprite_set_texture(&sprites[DARK_BULLET_MINI_LIGHT_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_BULLET_MINI_LIGHT_RED], 622, 16, 8, 8);
  
    Sprite_set_texture(&sprites[DARK_BULLET_MINI_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_BULLET_MINI_PINK], 622, 24, 8, 8);

    Sprite_set_texture(&sprites[DARK_BULLET_MINI_LIGHT_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_BULLET_MINI_LIGHT_PINK], 622, 32, 8, 8);

    Sprite_set_texture(&sprites[DARK_BULLET_MINI_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_BULLET_MINI_BLUE], 622, 40, 8, 8);

    Sprite_set_texture(&sprites[DARK_BULLET_MINI_LIGHT_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_BULLET_MINI_LIGHT_BLUE], 622, 48, 8, 8);

    Sprite_set_texture(&sprites[DARK_BULLET_MINI_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_BULLET_MINI_CYAN], 622, 56, 8, 8);

    Sprite_set_texture(&sprites[DARK_BULLET_MINI_LIGHT_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_BULLET_MINI_LIGHT_CYAN], 614, 0, 8, 8);

    Sprite_set_texture(&sprites[DARK_BULLET_MINI_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_BULLET_MINI_GREEN], 614, 8, 8, 8);

    Sprite_set_texture(&sprites[DARK_BULLET_MINI_LIGHT_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_BULLET_MINI_LIGHT_GREEN], 614, 16, 8, 8);

    Sprite_set_texture(&sprites[DARK_BULLET_MINI_LIME], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_BULLET_MINI_LIME], 614, 24, 8, 8); 

    Sprite_set_texture(&sprites[DARK_BULLET_MINI_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_BULLET_MINI_YELLOW], 614, 32, 8, 8);

    Sprite_set_texture(&sprites[DARK_BULLET_MINI_LIGHT_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_BULLET_MINI_LIGHT_YELLOW],  614, 40, 8, 8);

    Sprite_set_texture(&sprites[DARK_BULLET_MINI_GOLD], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_BULLET_MINI_GOLD], 614, 48, 8, 8);

    Sprite_set_texture(&sprites[DARK_BULLET_MINI_WHITE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[DARK_BULLET_MINI_WHITE], 614, 56, 8, 8);

    //MINI BULLETS
    Sprite_set_texture(&sprites[BULLET_MINI_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_MINI_BLACK], 574, 0, 8, 8);

    Sprite_set_texture(&sprites[BULLET_MINI_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_MINI_RED], 574, 8, 8, 8);

    Sprite_set_texture(&sprites[BULLET_MINI_LIGHT_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_MINI_LIGHT_RED], 574, 16, 8, 8);

    Sprite_set_texture(&sprites[BULLET_MINI_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_MINI_PINK], 574, 24, 8, 8);

    Sprite_set_texture(&sprites[BULLET_MINI_LIGHT_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_MINI_LIGHT_PINK], 574, 32, 8, 8);

    Sprite_set_texture(&sprites[BULLET_MINI_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_MINI_BLUE], 574, 40, 8, 8);

    Sprite_set_texture(&sprites[BULLET_MINI_LIGHT_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_MINI_LIGHT_BLUE], 574, 48, 8, 8);

    Sprite_set_texture(&sprites[BULLET_MINI_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_MINI_CYAN], 574, 56, 8, 8);

    Sprite_set_texture(&sprites[BULLET_MINI_LIGHT_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_MINI_LIGHT_CYAN], 566, 0, 8, 8);

    Sprite_set_texture(&sprites[BULLET_MINI_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_MINI_GREEN], 566, 8, 8, 8);

    Sprite_set_texture(&sprites[BULLET_MINI_LIGHT_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_MINI_LIGHT_GREEN], 566, 16, 8, 8);

    Sprite_set_texture(&sprites[BULLET_MINI_LIME], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_MINI_LIME], 566, 24, 8, 8);

    Sprite_set_texture(&sprites[BULLET_MINI_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_MINI_YELLOW], 566, 32, 8, 8);

    Sprite_set_texture(&sprites[BULLET_MINI_LIGHT_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_MINI_LIGHT_YELLOW], 566, 40, 8, 8);

    Sprite_set_texture(&sprites[BULLET_MINI_GOLD], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_MINI_GOLD], 566, 48, 8, 8);

    Sprite_set_texture(&sprites[BULLET_MINI_WHITE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_MINI_WHITE], 566, 56, 8, 8);

    //BULLET M
    Sprite_set_texture(&sprites[BULLET_M_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_M_BLACK], 568, 66, 12, 12);

    Sprite_set_texture(&sprites[BULLET_M_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_M_RED], 568, 82, 12, 12);

    Sprite_set_texture(&sprites[BULLET_M_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_M_BLUE], 568, 98, 12, 12);

    Sprite_set_texture(&sprites[BULLET_M_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_M_GREEN], 568, 114, 12, 12);

    Sprite_set_texture(&sprites[BULLET_M_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_M_YELLOW], 568, 130, 12, 12);

    Sprite_set_texture(&sprites[RING_M_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[RING_M_BLACK], 568, 146, 12, 12);

    Sprite_set_texture(&sprites[RING_M_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[RING_M_RED], 568, 162, 12, 12);

    Sprite_set_texture(&sprites[RING_M_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[RING_M_BLUE], 568, 178, 12, 12);

    Sprite_set_texture(&sprites[RING_M_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[RING_M_GREEN], 568, 194, 12, 12);

    Sprite_set_texture(&sprites[RING_M_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[RING_M_YELLOW], 568, 210, 12, 12);

    //BIG HEARTS
    Sprite_set_texture(&sprites[BIG_HEART_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BIG_HEART_BLACK], 524, 273, 30, 30);

    Sprite_set_texture(&sprites[BIG_HEART_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BIG_HEART_RED], 524, 305, 30, 30);

    Sprite_set_texture(&sprites[BIG_HEART_PINK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BIG_HEART_PINK], 524, 337, 30, 30);

    Sprite_set_texture(&sprites[BIG_HEART_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BIG_HEART_BLUE], 524, 369, 30, 30);

    Sprite_set_texture(&sprites[BIG_HEART_CYAN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BIG_HEART_CYAN], 524, 401, 30, 30);

    Sprite_set_texture(&sprites[BIG_HEART_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BIG_HEART_GREEN], 524, 433, 30, 30);

    Sprite_set_texture(&sprites[BIG_HEART_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BIG_HEART_YELLOW], 524, 465, 30, 30);
    
    Sprite_set_texture(&sprites[BIG_HEART_WHITE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BIG_HEART_WHITE], 524, 497, 30, 30);

    // BALL L
    Sprite_set_texture(&sprites[BALL_L_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_L_BLACK], 491, 2, 30, 30);

  }





void EnemiesSpritesLoad()
{

    // =========================
    // BLUE FAIRY
    // =========================

    Sprite_set_texture(&sprites[ENEMY_FAIRY_BLUE_IDLE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_BLUE_IDLE], 4, 0, 188, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_BLUE_IDLE], 4, 5);

    Sprite_set_texture(&sprites[ENEMY_FAIRY_BLUE_MOVE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_BLUE_MOVE], 0, 64, 188, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_BLUE_MOVE], 4, 5);


    // =========================
    // RED FAIRY
    // =========================

    Sprite_set_texture(&sprites[ENEMY_FAIRY_RED_IDLE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_RED_IDLE], 0, 95, 188, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_RED_IDLE], 4, 5);

    Sprite_set_texture(&sprites[ENEMY_FAIRY_RED_MOVE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_RED_MOVE], 0, 127, 188, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_RED_MOVE], 4, 5);

    // =========================
    // BLACK FAIRY BLONDE
    // =========================


    Sprite_set_texture(&sprites[ENEMY_FAIRY_BLACK_BLONDE_IDLE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_BLACK_BLONDE_IDLE], 0, 1674, 128, 28);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_BLACK_BLONDE_IDLE], 4, 5);

    // =========================
    // GREEN FAIRY
    // =========================

    Sprite_set_texture(&sprites[ENEMY_FAIRY_GREEN_IDLE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_GREEN_IDLE], 0, 959, 128, 28);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_GREEN_IDLE], 4, 5);

    // =========================
    // PURPLE FAIRY
    // =========================

    Sprite_set_texture(&sprites[ENEMY_FAIRY_PURPLE_IDLE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_PURPLE_IDLE], 0, 1706, 128, 28);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_PURPLE_IDLE], 4, 5);

    // =========================
    // SUNFLOWER FAIRY
    // =========================

    Sprite_set_texture(&sprites[ENEMY_FAIRY_BIG_SUNFLOWER_IDLE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_BIG_SUNFLOWER_IDLE], 0, 452, 256, 54);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_BIG_SUNFLOWER_IDLE], 4, 5);

    // =========================
    // DARK EVIL SPOOKY FAIRY 
    // =========================

    Sprite_set_texture(&sprites[ENEMY_FAIRY_BIG_EVIL_IDLE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_BIG_EVIL_IDLE], 0, 1739, 256, 54);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_BIG_EVIL_IDLE], 4, 5);

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
