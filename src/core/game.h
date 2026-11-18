#ifndef SP_GAME_H
#define SP_GAME_H

#include <stdint.h>

#include "core/game_state.h"

void game_init(GameState *state, uint32_t seed);
void game_restart(GameState *state);
void game_update(GameState *state, const Input *input);
const char *game_phase_name(GamePhase phase);

#endif /* SP_GAME_H */
