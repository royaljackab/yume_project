
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
#include "flags.h"
#include "screen.h"
#include "systems/obj.h"

#include "systems/score.h"
#include "tasks.h"
#include "common_task.h"
#include "screen_effects.h"

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

/* Macros definitions */
#define CREATE_PLAYER(numberBombs, playerSpeed, focus, radius, playerName) (Player){.nbBombs=numberBombs,     \
    .bombs=numberBombs,                                                                                 \
    .speed=playerSpeed,                                                                                   \
    .focusSpeed=focus,                                                                         \
    .GrazeSpriteId=-1,                                                                              \
    .hitboxSpriteId=-1,                                                                             \
    .grazeRadius=radius,                                                                       \
    .name=playerName                                                                                      \
}
#define GET_SPRITE_MOVEMENT(SpriteRootName, movement) sprites[SpriteRootName##_##movement]

/* Structures definitions */
typedef enum {
    IDLE,
    LEFT,
    RIGHT
} SpriteMovement;

/* Static variables */
static
Player test_player = CREATE_PLAYER(3, 7, 3.9, 20, TEST_PLAYER);

static bool is_input_locked = false;

/*Static functions*/
static
Player Player_create(PlayerName name) {
    is_input_locked = false;

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
            weapon = (Weapon){2, 0, 5, DEFAULT_PATTERN};
            break;
        default: break;
    }

    return weapon;
}

static
void Player_move(InputSystem *input, Pool *p, Entity player) {

    // TODO: Généraliser les sprites IDLE/LEFT

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
            Bullet_player_spawn(p, pos->pos.x - 10, pos->pos.y, 40, -90, REIMU_PINK_AMULET);
            Bullet_player_spawn(p, pos->pos.x + 10, pos->pos.y, 40, -90, REIMU_PINK_AMULET);
            // Son de tir
            if (!IsSoundPlaying(sfx[SFX_SHOOT])) {
                PlaySound(sfx[SFX_SHOOT]);
            }
        }
        weapon->cooldown = (weapon->cooldown + 1) % weapon->fire_rate;
    }
    
}

TASK(player_bomb_inv, {Pool *pool; Entity player;}) {
    Pool *p = ARGS.pool;
    Entity player = ARGS.player;

    obj_AddFlag(p, player, FLAG_INVINCIBLE);

    const int invi_duration = 120;

    for (int i=0; i<invi_duration; i++) {
        if (i % 8 < 4) obj_SetVisible(p, player, false);
        else obj_SetVisible(p, player, true);
        YIELD;
    }

    obj_SetVisible(p, player, true);
    obj_RemoveFlag(p, player, FLAG_INVINCIBLE);
}


extern void Player_bomb(GameContext *ctx, Entity player) {
    InputSystem *input = &ctx->input;
    Pool *p = ctx->pool;
    Position *pos = Position_get(&p->position, player);
    if (Player_get_bombs(Player_get(&p->player, player)) > 0) {
        if(input->bomb.isPressed) {
            Life_damage_all(p, 800);
            // Son de la bombe
            if (!IsSoundPlaying(sfx[SFX_BOMB])) {
                PlaySound(sfx[SFX_BOMB]);
            }
            Player_get(&p->player, player)->bombs--;
            SCHED_INVOKE_TASK(&ctx->sched, player_bomb_inv, ctx->pool, player);
            SCHED_INVOKE_TASK(&ctx->sched, orb_explosion_big, ctx->pool, pos->pos.x, pos->pos.y);
        }
    }
}

static void Player_focus(InputSystem *input, Pool *p, Entity player) {
    Player *player_p = Player_get(&p->player, player);
    Sprite * hitboxSprite = Sprite_get(&p->sprite,player_p->hitboxSpriteId);
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
}

void Player_graze(Pool *p, ScoreSystem *scoreS, Entity player){
    /**
     * @brief Gérer le graze du joueur
     * - incrémenter le score de graze et le score total en conséquence
     */
    flagList projectileFlag = {.flags = (FlagType[]){FLAG_PROJECTILE_ENEMY}, .size = 1};
    Player *player_p = Player_get(&p->player, player);
    Sprite * hitboxSprite = Sprite_get(&p->sprite,player_p->GrazeSpriteId);
    bool isGrazing = false;

    //check graze avec les hitbox cercles
    if(Entity_has_flag(p, player, FLAG_INVINCIBLE)){
        obj_SetVisible(p, player_p->GrazeSpriteId, false);
        return;
    }
    for(int i = 0; i < p->collision_circle.count; i++){
        Entity e = Collision_circle_get_entity(&p->collision_circle, i);
        if (!Entity_has_flag(p, e, FLAG_NO_DAMAGE_PLAYER) && Entity_has_flag_in_list(p, e, &projectileFlag) && CheckCollisionCircles(Position_get(&p->position, player)->pos, player_p->grazeRadius, obj_GetPosition(p, e), Collision_circle_get(&p->collision_circle, e)->radius)){
            score_increase(scoreS, scoreS->scoreOnGraze);
            scoreS->graze++;
            PlaySound(sfx[SFX_GRAZE]);
            obj_SetVisible(p, player_p->GrazeSpriteId, true);
            isGrazing = true;
        }
    }
    
    //check graze avec les hitbox rectangles (pour les lasers)
    for(int i = 0; i < p->collision_rectangle.count; i++){
        Entity e = Collision_rectangle_get_entity(&p->collision_rectangle, i);
        Collision_rectangle * cl = Collision_rectangle_get(&p->collision_rectangle, e);

        if (!Entity_has_flag(p, e, FLAG_NO_DAMAGE_PLAYER) && Entity_has_flag_in_list(p, e, &projectileFlag) && CheckCircleRotatedRect(Position_get(&p->position, player)->pos, player_p->grazeRadius, obj_GetPosition(p, e), cl->width, cl-> length, obj_GetAngle(p, e))){
            score_increase(scoreS, scoreS->scoreOnGraze);
            scoreS->graze++;
            PlaySound(sfx[SFX_GRAZE]);
            obj_SetVisible(p, player_p->GrazeSpriteId, true);
            isGrazing = true;
        }
    }
    if(!isGrazing){
        obj_SetVisible(p, player_p->GrazeSpriteId, false);
    }
}

/* Extern functions */
void Player_start(GameContext *ctx, PlayerName name, PatternType type) {
    Pool *p = ctx->pool;

    Player player = Player_create(name);
    Weapon weapon = Weapon_create(type);
    Sprite sprite = sprites[REIMU_IDLE];
    Tag tag = ENT_PLAYER;
    
    Entity e = pool_create_entity(p);
    Entity hitbox = particle_bound(p, HITBOX, e);
    Entity graze = particle_bound(p, GRAZE, e);
    obj_SetScale(p, graze, 1, 1);

    Player_add(&p->player, e, player);
    Weapon_add(&p->weapon, e, weapon);
    Position_add(&p->position, e, (Position){{0,0}, 0});
    Sprite_add(&p->sprite, e, sprite);
    Collision_circle_add(&p->collision_circle, e, (Collision_circle){6});

    Player_set_hitboxSpriteId(Player_get(&p->player,e),hitbox);
    Player_set_GrazeSpriteId(Player_get(&p->player,e), graze);

    Life_add(&p->life, e, (Life){INITIAL_PLAYER_LIVES, INITIAL_PLAYER_LIVES});
    Tag_add(&p->tag, e, tag);

    //ajout de la flagList du joueur
    FlagType * flagTypeList = malloc(sizeof(FlagType) * MAX_FLAGS);
    flagList flagList = {.flags = flagTypeList, .size = 0};
    flagList_add_element(&flagList, FLAG_PLAYER);
    flagList_add(&p->flagList, e, flagList);


    teleport_to_player_spawn(p, e);

    SCHED_INVOKE_TASK(&ctx->sched, reimu_yinyang_orb, ctx, e, 0);
    SCHED_INVOKE_TASK(&ctx->sched, reimu_yinyang_orb, ctx, e, 90);
    SCHED_INVOKE_TASK(&ctx->sched, reimu_yinyang_orb, ctx, e, -90);
    SCHED_INVOKE_TASK(&ctx->sched, reimu_yinyang_orb, ctx, e, 180);
}


void Player_update(GameContext *ctx) {
    Pool *p = ctx->pool;
    InputSystem *input = &ctx->input;

    Entity player = p->player.entity_lookup[0];
    
    if (!is_input_locked) {
        Player_move(input, p, player);
        Player_shoot(input, p, player);
        Player_bomb(ctx, player);
        Player_focus(input, p, player);
    }

    Damage_player_by_enemy_projectile(ctx, player);
    Player_graze(p, &ctx->score, player);
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

extern void  teleport_to_player_spawn(Pool *p, Entity e){
    Position_set_pos(Position_get(&p->position, e), (Vector2){PANEL_WIDTH/2, PANEL_HEIGHT*0.8});
}

TASK(player_respawn_sequence, {Pool *pool; Entity player;}) {
    Pool *p = ARGS.pool;
    Entity player = ARGS.player;

    is_input_locked = true;

    float px = obj_GetX(p, player);
    float py = obj_GetY(p, player);

    INVOKE_SUBTASK(orb_explosion, p, px, py);

    obj_SetVisible(p, player, false);
    obj_AddFlag(p, player, FLAG_INVINCIBLE);
    // Pour la hitbox parce que whatever.....
    Player *player_p = Player_get(&p->player, player);
    Sprite *hitboxSprite = Sprite_get(&p->sprite, player_p->hitboxSpriteId);
    if(hitboxSprite) hitboxSprite->display = false;

    WAIT(45);

    // LE joueur en bas là
    obj_SetPosition(p, player, PANEL_WIDTH / 2.0, PANEL_HEIGHT + 50);
    obj_SetVisible(p, player, true);

    // Le joueur se déplace jusqu'au spawn
    float target_y = PANEL_HEIGHT * 0.8;
    INVOKE_SUBTASK(obj_GoTo, p, player, PANEL_WIDTH / 2.0, target_y, 4);

    // Ca brille
    int frame = 0;
    while(obj_GetY(p, player) > target_y + 1.0f) {
        if (frame % 8 < 4) obj_SetVisible(p, player, false);
        else obj_SetVisible(p, player, true);
        frame++;
        YIELD;
    }

    // REPRISE DES CONTRÔLES 
    is_input_locked = false;

    // Invis encore
    for(int i = 0; i < 90; i++) {
        if (i % 8 < 4) obj_SetVisible(p, player, false);
        else obj_SetVisible(p, player, true);
        YIELD;
    }

    obj_SetVisible(p, player, true);
    obj_RemoveFlag(p, player, FLAG_INVINCIBLE);
}

extern bool Damage_player(GameContext *ctx, Entity player){
/** @brief Inflige des dégâts au joueur, et gère les conséquences de ces dégâts (perte de vie, combo, invincibilité temporaire, effets...)
 * @param ctx Le contexte du jeu, utilisé pour accéder au pool et au système de score
 * @param player L'entité du joueur à qui infliger les dégâts
 */
    Pool *p = ctx->pool;
    ctx->score.isComboActive = 0; //casse le combo actuelle
    Life *life = Life_get(&p->life, player);
    if (!life) return false;

    Life_damage(life, 1);
    PlaySound(sfx[SFX_PLDEAD00]);

    SCHED_INVOKE_TASK(&ctx->sched, player_hit_effect, ctx->pool, player);

    LifeManager *lm = &p->life;
    for (int i=0; i < lm->count; ++i) {
        Entity e = Life_get_entity(lm, i);
        Tag *tag = Tag_get(&p->tag, e);

        if (tag && *tag == ENT_PLAYER) continue;

        Life_damage(lm->dense + i, 1);
    }

    if (life->life > 0){
        SCHED_INVOKE_TASK(&ctx->sched, player_respawn_sequence, p, player);
    }
    return true;
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

DEFINE_EXTERN_TASK(reimu_yinyang_orb) {
    Pool *p = ARGS.ctx->pool;
    Entity player = ARGS.player;
    InputSystem* input = &ARGS.ctx->input;

    // Création de l'orbe
    Entity orb = pool_create_entity(p);

    Position pos = {{0,0}, 0};
    Position_add(&p->position, orb, pos);
    Sprite_add(&p->sprite, orb, sprites[YIN_YANG_ORB_RED]);

    float current_angle = ARGS.angle_offset;
    float current_radius = 130;
    int fire_timer = 0;

    while(true) {
        Vector2 p_pos = obj_GetPosition(p, player);

        bool player_is_visible = obj_IsVisible(p, player);
        if (!player_is_visible) obj_SetVisible(p, orb, false);
        else obj_SetVisible(p, orb, true);

        bool is_focus = isDown(input->focus);
        float target_radius = is_focus ? 40 : 130;

        current_radius += (target_radius - current_radius) * 0.15;

        current_angle += 7;

        //TRIGOOO
        float orb_x = p_pos.x + cosf(current_angle * DEG2RAD) * current_radius;
        float orb_y = p_pos.y + sinf(current_angle * DEG2RAD) * current_radius;

        obj_SetPosition(p, orb, orb_x, orb_y);
        obj_SetAngle(p, orb, obj_GetAngle(p, orb) - 8);

        // Quand le joueur tir
        if (isDown(input->shoot) && player_is_visible) {
            if (fire_timer % 3 == 0) {
                Bullet_player_spawn(p, orb_x - 4, orb_y, 30, -90, RED_STREAK);
                Bullet_player_spawn(p, orb_x + 4, orb_y, 30, -90, RED_STREAK);

            }
            fire_timer++;
        } else {
            fire_timer = 0;
        }

        YIELD;
    }
}