/*
 * GameState：由 main 持有并显式传递，core 内没有全局可变状态。
 */
#ifndef SP_GAME_STATE_H
#define SP_GAME_STATE_H

#include <stdbool.h>
#include <stdint.h>

#include "core/difficulty.h"
#include "core/input_abstraction.h"
#include "core/physics.h"
#include "core/score.h"
#include "core/terrain.h"

typedef enum GamePhase {
    GAME_PHASE_PLAYING = 0,
    GAME_PHASE_PAUSED = 1,
    GAME_PHASE_GAME_OVER = 2
} GamePhase;

typedef struct GameState {
    GamePhase phase;
    Player player;
    Terrain terrain;
    DifficultyParams difficulty;
    ScoreState score;
    Input last_input;

    uint32_t base_seed;
    uint32_t seed;
    uint32_t run_index;
    uint64_t fixed_steps;

    float elapsed_time;
    float distance_px;
    float world_speed;
    float death_time;

    int screen_width;
    int screen_height;
    float ground_y;
    bool debug_hud;
    bool quit_requested;

    int collision_segment;
    int collision_obstacle;
    int last_hit_type;
} GameState;

#endif /* SP_GAME_STATE_H */
