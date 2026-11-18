#include "core/game.h"

#include <string.h>

#include "core/collision.h"
#include "core/config.h"
#include "core/difficulty.h"
#include "core/input_abstraction.h"
#include "core/physics.h"
#include "core/terrain.h"

static void game_start_run(GameState *state)
{
    state->seed = state->base_seed + state->run_index * 0x9E3779B9u;
    state->run_index += 1u;

    state->phase = GAME_PHASE_PLAYING;
    state->elapsed_time = 0.0f;
    state->distance_px = 0.0f;
    state->death_time = 0.0f;
    state->fixed_steps = 0u;
    state->collision_segment = -1;
    state->collision_obstacle = -1;
    state->last_hit_type = -1;
    state->quit_requested = false;

    difficulty_init(&state->difficulty);
    state->world_speed = state->difficulty.speed;

    score_reset(&state->score);
    physics_reset(&state->player, SP_PLAYER_START_X, state->ground_y);
    terrain_init(&state->terrain, state->seed, state->ground_y, &state->difficulty);
    input_reset(&state->last_input);
}

void game_init(GameState *state, uint32_t seed)
{
    if (state == 0) {
        return;
    }

    memset(state, 0, sizeof(*state));
    state->base_seed = seed;
    state->run_index = 0u;
    state->screen_width = SP_SCREEN_WIDTH;
    state->screen_height = SP_SCREEN_HEIGHT;
    state->ground_y = SP_GROUND_Y;
    state->debug_hud = false;
    state->quit_requested = false;
    state->last_hit_type = -1;

    game_start_run(state);
}

void game_restart(GameState *state)
{
    int saved_high_score;
    bool saved_dirty;
    bool debug;

    if (state == 0) {
        return;
    }

    saved_high_score = state->score.high_score;
    saved_dirty = state->score.high_score_dirty;
    debug = state->debug_hud;

    game_start_run(state);

    state->score.high_score = saved_high_score;
    state->score.high_score_dirty = saved_dirty;
    state->debug_hud = debug;
}

void game_update(GameState *state, const Input *input)
{
    CollisionResult collision;
    float dx;

    if (state == 0 || input == 0) {
        return;
    }

    state->last_input = *input;

#if SP_DEBUG_HUD
    if (input->debug_pressed) {
        state->debug_hud = !state->debug_hud;
    }
#endif
    if (input->quit_pressed) {
        state->quit_requested = true;
        return;
    }

    if (state->phase == GAME_PHASE_GAME_OVER) {
        state->death_time += SP_FIXED_DT;
        if (input->restart_pressed || input->mouse_clicked) {
            game_restart(state);
        }
        return;
    }

    if (input->pause_pressed) {
        if (state->phase == GAME_PHASE_PLAYING) {
            state->phase = GAME_PHASE_PAUSED;
        } else if (state->phase == GAME_PHASE_PAUSED) {
            state->phase = GAME_PHASE_PLAYING;
        }
        return;
    }

    if (state->phase != GAME_PHASE_PLAYING) {
        return;
    }

    state->elapsed_time += SP_FIXED_DT;
    state->difficulty = difficulty_at(state->elapsed_time);
    state->world_speed = state->difficulty.speed;

    dx = state->world_speed * SP_FIXED_DT;
    state->distance_px += dx;
    terrain_scroll(&state->terrain, dx, &state->difficulty);

    physics_update(&state->player, input, state->ground_y, SP_FIXED_DT);
    state->player.run_phase += dx * 0.045f;

    score_update(&state->score, state->distance_px, state->elapsed_time);

    collision.hit = false;
    collision.segment_index = -1;
    collision.obstacle_index = -1;
    if (collision_check_player(&state->player, &state->terrain, &collision)) {
        state->phase = GAME_PHASE_GAME_OVER;
        state->death_time = 0.0f;
        state->collision_segment = collision.segment_index;
        state->collision_obstacle = collision.obstacle_index;
        if (collision.segment_index >= 0 && collision.obstacle_index >= 0) {
            state->last_hit_type =
                state->terrain.segments[collision.segment_index].obstacles[collision.obstacle_index].type;
        }
        score_finish(&state->score);
    }

    state->fixed_steps += 1u;
}

const char *game_phase_name(GamePhase phase)
{
    switch (phase) {
    case GAME_PHASE_PLAYING:
        return "PLAYING";
    case GAME_PHASE_PAUSED:
        return "PAUSED";
    case GAME_PHASE_GAME_OVER:
        return "GAME_OVER";
    default:
        return "UNKNOWN";
    }
}
