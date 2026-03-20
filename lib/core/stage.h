#include "game_state.h"
#include "components/player.h"


void stage_generic_update(GameContext *ctx);
void state_generic_init(GameContext *ctx, PlayerName player, PatternType type);
void stage_generic_draw(GameContext *ctx);
void stage_generic_cleanup(GameContext *ctx);