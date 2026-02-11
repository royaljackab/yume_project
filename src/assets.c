#include "../lib/assets.h"
#include "../lib/globals.h"

Texture2D textures[MAX_TEXTURES];
Music playlist[MAX_BGM]; // Nouveau tableau pour la musique

void AssetsLoad() {
    // Chargement des Textures existantes
    textures[BULLET_SPRITESHEET] = LoadTexture("../Assets/Sprites/bullet_spritesheet.png");
    textures[BOSS_SPRITESHEET] = LoadTexture("../Assets/Sprites/boss_spritesheet.png");
    textures[ANIM_TEST_SPRITESHEET] = LoadTexture("../Assets/Sprites/anim_test.png");
    textures[REIMU_SPRITESHEET] = LoadTexture("../Assets/Sprites/reimu_spritesheet.png");
    textures[HITBOX_SPRITESHEET] = LoadTexture("../Assets/Sprites/hitbox.png");

    // Chargement de la Musique
    // Note : On utilise LoadMusicStream pour les fichiers longs comme les BGM ok
    playlist[BGM_FAST_DANGER] = LoadMusicStream("../Assets/Audio/BGM/fast-danger.m4a");
    playlist[BGM_QUIRKY_GOOFY] = LoadMusicStream("../Assets/Audio/BGM/quirky-goofyr.m4a");
    playlist[BGM_TO_BE_NAMED] = LoadMusicStream("../Assets/Audio/BGM/tobenamedlater.m4a");
    playlist[BGM_WAITING] = LoadMusicStream("../Assets/Audio/BGM/waiting-settingstype.m4a");
}

void AssetsUnload() {
    // Déchargement des textures
    for(int i=0; i<MAX_TEXTURES; i++) UnloadTexture(textures[i]);

    // Déchargement des musiques (Libère la RAM de ton Mac/PC ok)
    for(int i=0; i<MAX_BGM; i++) UnloadMusicStream(playlist[i]);
}