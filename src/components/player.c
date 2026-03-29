
#include "components/player.h"
#include "components/bullet.h"
#include "components/common.h"
#include "components/life.h"
#include "components/physics.h"
#include "components/sprite.h"
#include "components/particle.h"
#include "components/collision_entity.h"

#include "ecs/pool.h"

#include "content/assets.h"

#include "core/game_state.h"
#include "core/input.h"
#include "core/screen.h"
#include "obj/obj.h"

#include <complex.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

/* Macros definitions */
#define CREATE_PLAYER(nbLives, nbBombs, speed, focusSpeed, name) (Player){nbLives, nbLives, nbBombs, nbBombs, speed, focusSpeed, -1, name}
#define GET_SPRITE_MOVEMENT(SpriteRootName, movement) sprites[SpriteRootName##_##movement]

/* Structures definitions */
typedef enum {
    IDLE,
    LEFT,
    RIGHT
} SpriteMovement;

/* Static variables */
static
Player test_player = CREATE_PLAYER(3, 3, 10, 7, TEST_PLAYER);

/*Static functions*/
static
Player Player_create(PlayerName name) {
    Player player;
    switch(name) {
        case TEST_PLAYER:
            player = test_player;
            break;
        default: break;
    }

    return player;
}

static
Weapon Weapon_create(PatternType type) {
    Weapon weapon;
    switch(type) {
        case DEFAULT_PATTERN:
            weapon = (Weapon){2, 0, DEFAULT_PATTERN};
            break;
        default: break;
    }

    return weapon;
}

static
void Player_move(InputSystem *input, Pool *p, Entity player) {

    // TODO: Généraliser les sprites IDLE/LEFT
    // TODO: focus mode

    Player *player_p = Player_get(&p->player, player);
    Position *pos = Position_get(&p->position, player);
    Sprite *sprite = Sprite_get(&p->sprite, player);
    Sprite new_sprite = sprites[REIMU_IDLE];

    Vector2 movement = {0,0};

    /* Bouger avec l'input */
    if (input->right.isDown) {
        movement.x ++;
    }
    if (input->left.isDown) {
        movement.x--;
    }
    if (input->up.isDown) {
        movement.y--;
    }
    if (input->down.isDown) {
        movement.y++;
    }
    movement = Vector2Normalize(movement);

    pos->pos = Vector2Add(pos->pos, Vector2Scale(movement,player_p->speed));

    /* Garder le joueur dans le cadre du jeu*/
    pos->pos.x = Clamp(pos->pos.x, 0, PANEL_WIDTH);
    pos->pos.y = Clamp(pos->pos.y, 0, PANEL_HEIGHT);

    /* Modifier le sprite en fonction du mouvement */
    if (movement.x > 0) {
        new_sprite = sprites[REIMU_RIGHT];
    } else if (movement.x < 0) {
        new_sprite = sprites[REIMU_RIGHT];
        new_sprite.srcRect.width *= -1;
    }
    if (sprite->animStart.x != new_sprite.animStart.x || sprite->animStart.y != new_sprite.animStart.y) {
        *sprite = new_sprite;
    }
}

static
void Player_shoot(InputSystem *input, Pool *p, Entity player) {

    Position *pos = Position_get(&p->position, player);
    Weapon *weapon = Weapon_get(&p->weapon, player);

    if(input->shoot.isDown) {
        if (weapon->cooldown == 0) {
            Bullet_player_spawn(p, pos->pos.x - 7, pos->pos.y, 40, -90, REIMU_PINK_AMULET);
            Bullet_player_spawn(p, pos->pos.x + 7, pos->pos.y, 40, -90, REIMU_PINK_AMULET);
            // Son de tir
            if (!IsSoundPlaying(sfx[SFX_SHOOT])) {
                PlaySound(sfx[SFX_SHOOT]);
            }
        }
        weapon->cooldown = (weapon->cooldown + 1) % weapon->fire_rate;
    }
    
}

static
void Player_focus(InputSystem *input, Pool *p, Entity player) {
    Player *player_p = Player_get(&p->player, player);
    Sprite * hitboxSprite = Sprite_get(&p->sprite,player_p->hitboxId);
    if(input->focus.isPressed){
        hitboxSprite->color.a = 0;
    }
    if(input->focus.isDown) {
        player_p->speed = player_p->focusSpeed;
        hitboxSprite->display = true;
        hitboxSprite->color.a = Clamp(hitboxSprite->color.a + 40, 0, 255);
        
    } 

    else {
        player_p->speed = test_player.speed;
        hitboxSprite->display = false;
        hitboxSprite->color.a = Clamp(hitboxSprite->color.a - 40, 0, 255);
    }

    


//       if (isPressed(ctx->input.focus)) {
//     playerSprites[HITBOX].color.a = 0;
//   }
//   if (isDown(ctx->input.focus)) {
//     speed = player.focusSpeed;
//     playerSprites[HITBOX].color.a =
//         Clamp(playerSprites[HITBOX].color.a + 40, 0, 255);
//   }
//   if (isReleased(ctx->input.focus)) {
//     playerSprites[HITBOX].color.a =
//         Clamp(playerSprites[HITBOX].color.a - 40, 0, 255);
//   }
}

/* Extern functions */
void Player_start(Pool *p, PlayerName name, PatternType type) {
    Player player = Player_create(name);
    Weapon weapon = Weapon_create(type);
    Sprite sprite = sprites[REIMU_IDLE];
    
    Entity e = pool_create_entity(p);
    Entity hitbox = particle_bound(p, HITBOX, e);

    Player_add(&p->player, e, player);
    Weapon_add(&p->weapon, e, weapon);
    Position_add(&p->position, e, (Position){{0,0}, 0});
    Sprite_add(&p->sprite, e, sprite);
    Collision_circle_add(&p->collision_circle, e, (Collision_circle){5});
    Player_set_hitboxId(Player_get(&p->player,e),hitbox);
    Life_add(&p->life, e, (Life){INITIAL_PLAYER_LIVES, INITIAL_PLAYER_LIVES});

        //ajout de la flagList du joueur
    FlagType * flagTypeList = malloc(sizeof(FlagType) * MAX_FLAGS);
    flagList flagList = {.flags = flagTypeList, .size = 0};
    flagList_add_element(&flagList, FLAG_PLAYER);
    flagList_add(&p->flagList, e, flagList);


    teleport_to_player_spawn(p, e);

}


void Player_update(GameContext *ctx) {
    Pool *p = ctx->pool;
    InputSystem *input = &ctx->input;

    Entity player = p->player.entity_lookup[0];
    
    Player_move(input, p, player);
    Player_shoot(input, p, player);
    Player_focus(input, p, player);
}

Entity Player_get_playerID(Pool *pool) {
    return pool->player.entity_lookup[0];
}

float Player_GetX(Pool *pool) {
    Entity player = Player_get_playerID(pool);
    return obj_GetX(pool, player);
}

float Player_GetY(Pool *pool) {
    Entity player = Player_get_playerID(pool);
    return obj_GetY(pool, player);
}
    Damage_player_by_enemy_projectile(p, player);
    
}

extern void  teleport_to_player_spawn(Pool *p, Entity e){
    Position_set_pos(Position_get(&p->position, e), (Vector2){PANEL_WIDTH/2, PANEL_HEIGHT*0.8});
}

extern bool Damage_player(Pool *p, Entity player){
    Life *life = Life_get(&p->life, player);
    if (!life) return false;
    Life_damage(life, 1);
    if (life->life > 0){
        teleport_to_player_spawn(p, player);
        //make_player_incinvible(p, player);
        //clear_screan_projectiles(p); 
    }
}

extern Position * Player_get_position(Pool *p, Player player){
    /**
     * Récupère la position a partir d'un joueur
     * Actuellement, renvoie celle du joueur de base, pas du paramètre
     */
  PositionManager *positionManager = &p->position;
  int lookup = positionManager->entity_lookup[0];
  return &positionManager->dense[lookup];

}

extern Collision_circle * Player_get_collision(Pool *p, Player player){
    /**
     * Récupère le cercle de collision a partir d'un joueur
     * Actuellement, renvoie celle du joueur de base, pas du paramètre
     */
  Collision_circleManager *collision_circleManager = &p->collision_circle;
  int lookup = collision_circleManager->entity_lookup[0];
  return &collision_circleManager->dense[lookup];
}

