#include "game_state.h"
#include "components/player.h"



#define NO_WAVE -1
#define DOWNTIME 0

#define BOSS_WAVE 10000
#define STAGE_FINISHED 10001


typedef struct {
    int current_wave; /**< Vague actuelle du stage */
} StageSystem;

void stage_generic_update(GameContext *ctx);
void stage_generic_init(GameContext *ctx, PlayerName player, PatternType type);
void stage_generic_draw(GameContext *ctx);
void stage_generic_cleanup(GameContext *ctx);