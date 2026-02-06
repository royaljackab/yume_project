//TODO: Eliminer le fichier décadent
////// DEPRECATED //////

#include "../lib/player.h"
#include "../lib/input.h"
#include "../lib/game_state.h"

Player player;

Sprite playerSprites[10];
Player characters[4];

int shotTimer = 0;

void Load_PlayerSprites() {
  SetTexture(&playerSprites[REIMU_IDLE], REIMU_SPRITESHEET);
  SetSourceRect(&playerSprites[REIMU_IDLE], 0, 0, 256, 48);
  SetAnimation(&playerSprites[REIMU_IDLE], 8, 5);
  SetCollisionRadius(&playerSprites[REIMU_IDLE], 5);

  SetTexture(&playerSprites[REIMU_LEFT], REIMU_SPRITESHEET);
  SetSourceRect(&playerSprites[REIMU_LEFT], 0, 48, 256, 48);
  SetAnimation(&playerSprites[REIMU_LEFT], 8, 5);
  SetCollisionRadius(&playerSprites[REIMU_LEFT], 5);

  SetTexture(&playerSprites[HITBOX], HITBOX_SPRITESHEET);
  SetSourceRect(&playerSprites[HITBOX], 0, 0, 64, 64);
  SetCollisionRadius(&playerSprites[HITBOX], 3);
  SetRotation(&playerSprites[HITBOX], 0);
  playerSprites[HITBOX].color.a = 0;
}

void Load_Players() {
  characters[REIMU_A] = (Player){playerSprites[REIMU_IDLE],
                                 (Vector2){300, 800},
                                 7,
                                 4,
                                 3,
                                 3,
                                 3,
                                 3,
                                 REIMU_A};
}

void InitPlayer(int playerType) { player = characters[playerType]; }

void UpdatePlayer(GameContext *ctx) {

  // Vitesse + hitbox (focus mode)
  float speed = player.speed;
  if (isPressed(ctx->input.focus)) {
    playerSprites[HITBOX].color.a = 0;
  }
  if (isDown(ctx->input.focus)) {
    speed = player.focusSpeed;
    playerSprites[HITBOX].color.a =
        Clamp(playerSprites[HITBOX].color.a + 40, 0, 255);
  }
  if (isReleased(ctx->input.focus)) {
    playerSprites[HITBOX].color.a =
        Clamp(playerSprites[HITBOX].color.a - 40, 0, 255);
  }

  // Tirs
  if (isDown(ctx->input.shoot)) {
    if (shotTimer == 0) {
      CreateShotType(OBJ_PLAYER_SHOT, player.pos.x - 7, player.pos.y - 20, 50,
                     270, REIMU_PINK_AMULET, 0);
      CreateShotType(OBJ_PLAYER_SHOT, player.pos.x + 7, player.pos.y - 20, 50,
                     270, REIMU_PINK_AMULET, 0);
    }
    shotTimer = (shotTimer + 1) % 2;
  }

  Vector2 move = {0, 0};

  // Déplacements
  if (isDown(ctx->input.left))
    move = Vector2Add(move, (Vector2){-1, 0});
  if (isDown(ctx->input.up))
    move = Vector2Add(move, (Vector2){0, -1});
  if (isDown(ctx->input.right))
    move = Vector2Add(move, (Vector2){1, 0});
  if (isDown(ctx->input.down))
    move = Vector2Add(move, (Vector2){0, 1});

  // Anim
  int currentAnim = REIMU_IDLE;
  if (player.sprite.srcRect.y == 48) {
    if (player.sprite.srcRect.width < 0)
      currentAnim = REIMU_RIGHT;
    else
      currentAnim = REIMU_LEFT;
  }

  int targetAnim = REIMU_IDLE;
  if (move.x < 0)
    targetAnim = REIMU_LEFT;
  else if (move.x > 0)
    targetAnim = REIMU_RIGHT;

  if (currentAnim != targetAnim) {
    if (targetAnim == REIMU_IDLE) {
      player.sprite = playerSprites[REIMU_IDLE];
    } else if (targetAnim == REIMU_LEFT) {
      player.sprite = playerSprites[REIMU_LEFT];
      if (player.sprite.srcRect.width < 0)
        player.sprite.srcRect.width *= -1;
    } else if (targetAnim == REIMU_RIGHT) {
      player.sprite = playerSprites[REIMU_LEFT];
      if (player.sprite.srcRect.width > 0)
        player.sprite.srcRect.width *= -1;
    }
  }

  move = Vector2Normalize(move);
  player.pos = Vector2Add(player.pos, Vector2Scale(move, speed));
  player.pos.x = Clamp(player.pos.x, PANEL_LEFT, PANEL_LEFT + PANEL_WIDTH);
  player.pos.y = Clamp(player.pos.y, PANEL_UP, PANEL_UP + PANEL_HEIGHT);
}

void DrawPlayer() {
  DrawSprite(player.sprite, player.pos);

  // Affichage hitbox
  if (playerSprites[HITBOX].color.a > 0) {
    DrawSprite(playerSprites[HITBOX], player.pos);
    playerSprites[HITBOX].rotation += 3;
  }
}

void UpdateAnimationPlayer() { UpdateAnimation(&player.sprite); }
