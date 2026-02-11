#ifndef SPRITE_H
#define SPRITE_H

#include "raylib.h"
#include <stdbool.h>
#include "component.h"
#include "assets.h"
#include "common.h"

typedef struct {
    int textureID;
    Rectangle srcRect;
    Vector2 center;
    Vector2 scale;
    Color color;
    int renderPriority; //Pour afficher devant/derriere d'autres..
    int display;

    //Pour l'animation
    bool isAnimated;
    int animFrameCount;
    int animSpeed;
    int animTimer;
    int currentFrame;
    int frameWidth;
    float rotation;
    Vector2 animStart;

} Sprite;

DEFINE_COMPONENT_MANAGER(Sprite, MAX_ENTITIES)
DECLARE_SETTER_GETTER(Sprite, int, textureID)
DECLARE_SETTER_GETTER(Sprite, Rectangle, srcRect)
DECLARE_SETTER_GETTER(Sprite, Vector2, center)
DECLARE_SETTER_GETTER(Sprite, float, rotation)
DECLARE_SETTER_GETTER(Sprite, Vector2, scale)
DECLARE_SETTER_GETTER(Sprite, Color, color)
DECLARE_SETTER_GETTER(Sprite, int, renderPriority)
DECLARE_SETTER_GETTER(Sprite, int, display)
DECLARE_SETTER_GETTER(Sprite, bool, isAnimated)
DECLARE_SETTER_GETTER(Sprite, int, animFrameCount)
DECLARE_SETTER_GETTER(Sprite, int, animSpeed)
DECLARE_SETTER_GETTER(Sprite, int, animTimer)
DECLARE_SETTER_GETTER(Sprite, int, currentFrame)
DECLARE_SETTER_GETTER(Sprite, int, frameWidth)
DECLARE_SETTER_GETTER(Sprite, Vector2, animStart)

void SetSourceRect     (Sprite* sprite, float x, float y, float width, float height);
void SetAnimation      (Sprite* sprite, int frameCount, int delay);
void UpdateAnimation   (Sprite* sprite);
void DrawSprite        (Sprite sprite, Vector2 pos);
void drawAll           (SpriteManager * spriteManager, PositionManager * commonManager);

#endif


