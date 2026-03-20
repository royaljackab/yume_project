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

    Sprite_set_texture(&sprites[REIMU_IDLE], RENDER_PRIO_PLAYER, REIMU_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[REIMU_IDLE], 0, 0, 256, 48);
    Sprite_set_animation(&sprites[REIMU_IDLE], 8, 5);

    Sprite_set_texture(&sprites[REIMU_LEFT], RENDER_PRIO_PLAYER, REIMU_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[REIMU_LEFT], 0, 48, 256, 48);
    Sprite_set_animation(&sprites[REIMU_LEFT], 8, 5);

    Sprite_set_texture(&sprites[REIMU_RIGHT], RENDER_PRIO_PLAYER, REIMU_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[REIMU_RIGHT], 0, 96, 256, 48);
    Sprite_set_animation(&sprites[REIMU_RIGHT], 8, 5);

    Sprite_set_texture(&sprites[REIMU_PINK_AMULET], RENDER_PRIO_PLAYER, REIMU_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[REIMU_PINK_AMULET], 138, 178, 56, 14);
    Sprite_set_center(&sprites[REIMU_PINK_AMULET], (Vector2){46,6});
    Sprite_set_rotation(&sprites[REIMU_PINK_AMULET], -90);

    Sprite_set_texture(&sprites[HITBOX], RENDER_PRIO_HITBOX, HITBOX_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[HITBOX], 0, 0, 64, 64);
    Sprite_set_rotation(&sprites[HITBOX], 0);


    BulletsSpritesLoad();
}

void BulletsSpritesLoad() {
    /**
     *  Fonction intermediaire pour load tous les assets du jeu.
     *  Load toutes les bullets.
     **/
    Sprite_set_texture(&sprites[NULL_SPRITE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[NULL_SPRITE], 0, 0, 0, 0);


    Sprite_set_texture(&sprites[BALL_M_BLACK], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BALL_M_BLACK], 5, 54, 16, 16);

    // SMALL BULLETS (16x16)

    Sprite_set_texture(&sprites[BULLET_SMALL_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_SMALL_RED], 6, 6, 16, 16);

    Sprite_set_texture(&sprites[BULLET_SMALL_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_SMALL_BLUE], 38, 6, 16, 16);

    Sprite_set_texture(&sprites[BULLET_SMALL_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_SMALL_GREEN], 70, 6, 16, 16);

    Sprite_set_texture(&sprites[BULLET_SMALL_YELLOW], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_SMALL_YELLOW], 102, 6, 16, 16);


    // RING BULLETS

    Sprite_set_texture(&sprites[BULLET_RING_RED], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_RED], 8, 210, 32, 32);

    Sprite_set_texture(&sprites[BULLET_RING_BLUE], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_BLUE], 44, 210, 32, 32);

    Sprite_set_texture(&sprites[BULLET_RING_GREEN], RENDER_PRIO_BULLET, BULLET_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[BULLET_RING_GREEN], 80, 210, 32, 32);


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

    Sprite_set_texture(&sprites[ENEMY_FAIRY_BLUE_IDLE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_BLUE_IDLE], 0, 0, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_BLUE_IDLE], 4, 5);

    Sprite_set_texture(&sprites[ENEMY_FAIRY_BLUE_MOVE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_BLUE_MOVE], 0, 32, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_BLUE_MOVE], 4, 5);


    // =========================
    // RED FAIRY
    // =========================

    Sprite_set_texture(&sprites[ENEMY_FAIRY_RED_IDLE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_RED_IDLE], 128, 0, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_RED_IDLE], 4, 5);

    Sprite_set_texture(&sprites[ENEMY_FAIRY_RED_MOVE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_RED_MOVE], 128, 32, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_RED_MOVE], 4, 5);


    // =========================
    // CYAN FAIRY
    // =========================

    Sprite_set_texture(&sprites[ENEMY_FAIRY_CYAN_IDLE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_CYAN_IDLE], 256, 0, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_CYAN_IDLE], 4, 5);

    Sprite_set_texture(&sprites[ENEMY_FAIRY_CYAN_MOVE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_CYAN_MOVE], 256, 32, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_CYAN_MOVE], 4, 5);


    // =========================
    // GREEN FAIRY
    // =========================

    Sprite_set_texture(&sprites[ENEMY_FAIRY_GREEN_IDLE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_GREEN_IDLE], 384, 0, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_GREEN_IDLE], 4, 5);

    Sprite_set_texture(&sprites[ENEMY_FAIRY_GREEN_MOVE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_GREEN_MOVE], 384, 32, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_GREEN_MOVE], 4, 5);


    // =========================
    // YELLOW FAIRY
    // =========================

    Sprite_set_texture(&sprites[ENEMY_FAIRY_YELLOW_IDLE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_YELLOW_IDLE], 0, 64, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_YELLOW_IDLE], 4, 5);

    Sprite_set_texture(&sprites[ENEMY_FAIRY_YELLOW_MOVE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_FAIRY_YELLOW_MOVE], 0, 96, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_FAIRY_YELLOW_MOVE], 4, 5);


    // =========================
    // BUTTERFLY BLUE
    // =========================

    Sprite_set_texture(&sprites[ENEMY_BUTTERFLY_BLUE_IDLE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_BUTTERFLY_BLUE_IDLE], 128, 256, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_BUTTERFLY_BLUE_IDLE], 4, 6);

    Sprite_set_texture(&sprites[ENEMY_BUTTERFLY_BLUE_MOVE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_BUTTERFLY_BLUE_MOVE], 128, 288, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_BUTTERFLY_BLUE_MOVE], 4, 6);


    // =========================
    // BUTTERFLY RED
    // =========================

    Sprite_set_texture(&sprites[ENEMY_BUTTERFLY_RED_IDLE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_BUTTERFLY_RED_IDLE], 256, 256, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_BUTTERFLY_RED_IDLE], 4, 6);

    Sprite_set_texture(&sprites[ENEMY_BUTTERFLY_RED_MOVE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_BUTTERFLY_RED_MOVE], 256, 288, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_BUTTERFLY_RED_MOVE], 4, 6);


    // =========================
    // BIG FAIRY
    // =========================

    Sprite_set_texture(&sprites[ENEMY_BIG_FAIRY_IDLE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
    Sprite_set_SourceRect(&sprites[ENEMY_BIG_FAIRY_IDLE], 0, 512, 128, 32);
    Sprite_set_animation(&sprites[ENEMY_BIG_FAIRY_IDLE], 4, 4);

    Sprite_set_texture(&sprites[ENEMY_BIG_FAIRY_MOVE], RENDER_PRIO_ENEMY, ENEMY_SPRITESHEET);
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
