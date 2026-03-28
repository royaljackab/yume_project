/**
 * @file assets.c
 * @author jack abribat
 * description blah blah
 */

#include "content/assets.h"
#include "sprite.h"
#include <raylib.h>

Sprite sprites[MAX_SPRITES];
Texture2D textures[MAX_TEXTURES];
Music playlist[MAX_BGM]; // Nouveau tableau pour la musique
Sound sfx[MAX_SFX]; // Effets sonores

enum{ 
  RENDER_PRIO_BULLET, 

  RENDER_PRIO_PLAYER, 
  RENDER_PRIO_HITBOX, 

  RENDER_PRIO_ENEMY, 
};

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

    // Load dem enemies up
    EnemiesSpritesLoad();
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
    Sprite_set_texture(&sprites[BULLET_RING_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_RED], 38, 215, 32, 32);

    Sprite_set_texture(&sprites[BULLET_RING_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_RED], 8, 210, 32, 32);

    Sprite_set_texture(&sprites[BULLET_RING_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_BLUE], 44, 210, 32, 32);

    Sprite_set_texture(&sprites[BULLET_RING_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_GREEN], 80, 210, 32, 32);

    Sprite_set_texture(&sprites[BULLET_RING_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_GREEN], 166, 215, 32, 32);

    // STAR BULLETS
    Sprite_set_texture(&sprites[BULLET_STAR_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_STAR_RED], 6, 330, 32, 32);

    Sprite_set_texture(&sprites[BULLET_STAR_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_STAR_BLUE], 42, 330, 32, 32);

    // BIG BALL BULLETS
    Sprite_set_texture(&sprites[BULLET_BIG_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_BIG_RED], 310, 40, 64, 64);

    Sprite_set_texture(&sprites[BULLET_BIG_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_BIG_BLUE], 378, 40, 64, 64);

    // FIRE BULLETS
    Sprite_set_texture(&sprites[BULLET_FIRE_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_FIRE_RED], 240, 260, 64, 64);

    Sprite_set_texture(&sprites[BULLET_FIRE_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_FIRE_BLUE], 310, 260, 64, 64);

    Sprite_set_texture(&sprites[BULLET_FIRE_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_FIRE_GREEN], 380, 260, 64, 64);

    // GRADIENT BULLETS
    Sprite_set_texture(&sprites[BULLET_BLACK_GRADIENT], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_BLACK_GRADIENT], 8, 470, 64, 64);

    Sprite_set_texture(&sprites[BULLET_PURPLE_GRADIENT], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_PURPLE_GRADIENT], 78, 470, 64, 64);

    Sprite_set_texture(&sprites[BULLET_GREEN_GRADIENT], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_GREEN_GRADIENT], 148, 470, 64, 64);

    Sprite_set_texture(&sprites[BULLET_YELLOW_GRADIENT], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_YELLOW_GRADIENT], 218, 470, 64, 64);
}




void EnemiesSpritesLoad()
{

    // =========================
    // BLUE FAIRY
    // =========================

    Sprite_set_texture(&sprites[ENEMY_FAIRY_BLUE_IDLE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_BLUE_IDLE], 0, 0, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_BLUE_IDLE], 4, 5);

    Sprite_set_texture(&sprites[ENEMY_FAIRY_BLUE_MOVE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_BLUE_MOVE], 0, 32, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_BLUE_MOVE], 4, 5);


    // =========================
    // RED FAIRY
    // =========================

    Sprite_set_texture(&sprites[ENEMY_FAIRY_RED_IDLE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_RED_IDLE], 128, 0, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_RED_IDLE], 4, 5);

    Sprite_set_texture(&sprites[ENEMY_FAIRY_RED_MOVE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_RED_MOVE], 128, 32, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_RED_MOVE], 4, 5);


    // =========================
    // CYAN FAIRY
    // =========================

    Sprite_set_texture(&sprites[ENEMY_FAIRY_CYAN_IDLE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_CYAN_IDLE], 256, 0, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_CYAN_IDLE], 4, 5);

    Sprite_set_texture(&sprites[ENEMY_FAIRY_CYAN_MOVE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_CYAN_MOVE], 256, 32, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_CYAN_MOVE], 4, 5);


    // =========================
    // GREEN FAIRY
    // =========================

    Sprite_set_texture(&sprites[ENEMY_FAIRY_GREEN_IDLE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_GREEN_IDLE], 384, 0, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_GREEN_IDLE], 4, 5);

    Sprite_set_texture(&sprites[ENEMY_FAIRY_GREEN_MOVE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_GREEN_MOVE], 384, 32, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_GREEN_MOVE], 4, 5);


    // =========================
    // YELLOW FAIRY
    // =========================

    Sprite_set_texture(&sprites[ENEMY_FAIRY_YELLOW_IDLE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_YELLOW_IDLE], 0, 64, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_YELLOW_IDLE], 4, 5);

    Sprite_set_texture(&sprites[ENEMY_FAIRY_YELLOW_MOVE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_YELLOW_MOVE], 0, 96, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_YELLOW_MOVE], 4, 5);


    // =========================
    // BUTTERFLY BLUE
    // =========================

    Sprite_set_texture(&sprites[ENEMY_BUTTERFLY_BLUE_IDLE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_BUTTERFLY_BLUE_IDLE], 128, 256, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_BUTTERFLY_BLUE_IDLE], 4, 6);

    Sprite_set_texture(&sprites[ENEMY_BUTTERFLY_BLUE_MOVE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_BUTTERFLY_BLUE_MOVE], 128, 288, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_BUTTERFLY_BLUE_MOVE], 4, 6);


    // =========================
    // BUTTERFLY RED
    // =========================

    Sprite_set_texture(&sprites[ENEMY_BUTTERFLY_RED_IDLE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_BUTTERFLY_RED_IDLE], 256, 256, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_BUTTERFLY_RED_IDLE], 4, 6);

    Sprite_set_texture(&sprites[ENEMY_BUTTERFLY_RED_MOVE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_BUTTERFLY_RED_MOVE], 256, 288, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_BUTTERFLY_RED_MOVE], 4, 6);


    // =========================
    // BIG FAIRY
    // =========================

    Sprite_set_texture(&sprites[ENEMY_BIG_FAIRY_IDLE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_BIG_FAIRY_IDLE], 0, 512, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_BIG_FAIRY_IDLE], 4, 4);

    Sprite_set_texture(&sprites[ENEMY_BIG_FAIRY_MOVE], 0, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_BIG_FAIRY_MOVE], 0, 544, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_BIG_FAIRY_MOVE], 4, 4);

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
