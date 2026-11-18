#include <math.h>

#include "core/config.h"
#include "core/game.h"
#include "core/input_abstraction.h"

#include "test_framework.h"

static int nearest_obstacle_dx(const GameState *state, float *out_dx, float *out_width,
                               float *out_height)
{
    const Player *player = &state->player;
    float best = 1000000.0f;
    int best_segment = -1;
    int found = 0;
    int i;
    int j;

    for (i = 0; i < state->terrain.segment_count; ++i) {
        const TerrainSegment *segment = &state->terrain.segments[i];
        for (j = 0; j < segment->obstacle_count; ++j) {
            const Obstacle *obstacle = &segment->obstacles[j];
            float dx;
            if (obstacle->x + obstacle->w <= player->x) {
                continue;
            }
            dx = obstacle->x - (player->x + SP_PLAYER_W);
            if (dx < best) {
                best = dx;
                best_segment = i;
                found = 1;
            }
        }
    }
    if (found) {
        const TerrainSegment *segment = &state->terrain.segments[best_segment];
        float nearest_x = player->x + SP_PLAYER_W + best;
        float rightmost = nearest_x;
        float highest = 0.0f;
        for (j = 0; j < segment->obstacle_count; ++j) {
            float right = segment->obstacles[j].x + segment->obstacles[j].w;
            if (right > rightmost) {
                rightmost = right;
            }
            if (segment->obstacles[j].h > highest) {
                highest = segment->obstacles[j].h;
            }
        }
        *out_dx = best;
        *out_width = rightmost - nearest_x;
        *out_height = highest;
    }
    return found;
}

static void run_autoplay(GameState *state, int total_steps, int *steps_run)
{
    int i;
    int jump_count = 0;

    for (i = 0; i < total_steps; ++i) {
        Input input;
        float dx = 0.0f;
        float obstacle_width = 0.0f;
        float obstacle_height = 0.0f;

        input_reset(&input);
        if (nearest_obstacle_dx(state, &dx, &obstacle_width, &obstacle_height)) {
            const float jump_speed = 880.0f;
            const float gravity = 2100.0f;
            float target_height = obstacle_height + 12.0f;
            float discriminant = jump_speed * jump_speed - 2.0f * gravity * target_height;
            float t_fall = (2.0f * jump_speed) / gravity;
            float player_box_width = SP_PLAYER_W - 10.0f;
            float max_jump_dx;

            if (discriminant > 0.0f) {
                t_fall = (jump_speed + sqrtf(discriminant)) / gravity;
            }
            t_fall -= 0.04f;
            if (t_fall < 0.08f) {
                t_fall = 0.08f;
            }
            max_jump_dx = state->world_speed * t_fall - player_box_width - obstacle_width - 8.0f;
            if (max_jump_dx < 45.0f) {
                max_jump_dx = 45.0f;
            }
            if (state->player.on_ground && dx > 10.0f && dx < max_jump_dx) {
                input.jump_pressed = true;
                jump_count += 1;
            }
        }
        game_update(state, &input);
        if (state->phase == GAME_PHASE_GAME_OVER) {
            int k;
            printf("  death detail: t=%.2f jumps=%d py=%.1f vy=%.1f nearest=%.1f jumps_used=%d\n",
                   (double)state->elapsed_time, jump_count, (double)state->player.y,
                   (double)state->player.vy, (double)dx, state->player.jumps_used);
            for (k = 0; k < state->terrain.segment_count; ++k) {
                const TerrainSegment *sg = &state->terrain.segments[k];
                int m;
                printf("    seg[%d] x=%.1f w=%.1f obs=%d", k, (double)sg->x,
                       (double)sg->width, sg->obstacle_count);
                for (m = 0; m < sg->obstacle_count; ++m) {
                    printf(" (%d x=%.1f w=%.1f h=%.1f)", sg->obstacles[m].type,
                           (double)sg->obstacles[m].x, (double)sg->obstacles[m].w,
                           (double)sg->obstacles[m].h);
                }
                printf("\n");
            }
            break;
        }
    }
    if (steps_run != 0) {
        *steps_run = i;
    }
}

void test_game_suite(void)
{
    GameState state;
    Input input;
    int i;

    game_init(&state, 0x5EED2026u);
    TF_CHECK(state.phase == GAME_PHASE_PLAYING);
    TF_CHECK(state.player.on_ground);
    TF_CHECK(state.terrain.segment_count > 0);
    TF_CHECK_INT(state.run_index, 1u);
    TF_CHECK_NEAR(state.player.y, SP_GROUND_Y - SP_PLAYER_H, 0.01f);

    /* 暂停期间时间与距离冻结。 */
    input_reset(&input);
    input.pause_pressed = true;
    game_update(&state, &input);
    TF_CHECK(state.phase == GAME_PHASE_PAUSED);
    {
        float frozen_time = state.elapsed_time;
        float frozen_distance = state.distance_px;
        input_reset(&input);
        for (i = 0; i < 30; ++i) {
            game_update(&state, &input);
        }
        TF_CHECK_NEAR(state.elapsed_time, frozen_time, 0.0001f);
        TF_CHECK_NEAR(state.distance_px, frozen_distance, 0.0001f);
    }
    input_reset(&input);
    input.pause_pressed = true;
    game_update(&state, &input);
    TF_CHECK(state.phase == GAME_PHASE_PLAYING);

    /* F1 调试开关。 */
    input_reset(&input);
    input.debug_pressed = true;
    game_update(&state, &input);
    TF_CHECK(state.debug_hud);
    input_reset(&input);
    input.debug_pressed = true;
    game_update(&state, &input);
    TF_CHECK(!state.debug_hud);

    /* 无输入必撞上第一个障碍。 */
    game_init(&state, 42u);
    input_reset(&input);
    for (i = 0; i < 900 && state.phase == GAME_PHASE_PLAYING; ++i) {
        game_update(&state, &input);
    }
    TF_CHECK(state.phase == GAME_PHASE_GAME_OVER);
    TF_CHECK(state.score.high_score > 0);
    TF_CHECK(state.score.high_score_dirty);
    TF_CHECK(state.score.new_record);
    TF_CHECK(state.death_time >= 0.0f);

    {
        int saved_high_score = state.score.high_score;
        uint32_t saved_run = state.run_index;
        input_reset(&input);
        game_update(&state, &input);
        TF_CHECK(state.death_time > 0.0f);
        game_restart(&state);
        TF_CHECK(state.phase == GAME_PHASE_PLAYING);
        TF_CHECK_INT(state.score.current, 0);
        TF_CHECK_INT(state.score.high_score, saved_high_score);
        TF_CHECK(state.run_index > saved_run);
        TF_CHECK(state.player.on_ground);
    }

    /* ESC 退出请求。 */
    input_reset(&input);
    input.quit_pressed = true;
    game_update(&state, &input);
    TF_CHECK(state.quit_requested);

    /* 同种子、同输入 -> 同结果。 */
    {
        GameState a;
        GameState b;
        input_reset(&input);
        game_init(&a, 0xABCDEF01u);
        game_init(&b, 0xABCDEF01u);
        for (i = 0; i < 600; ++i) {
            game_update(&a, &input);
            game_update(&b, &input);
        }
        TF_CHECK(a.phase == b.phase);
        TF_CHECK_INT(a.fixed_steps, b.fixed_steps);
        TF_CHECK_INT(a.collision_segment, b.collision_segment);
        TF_CHECK_INT(a.collision_obstacle, b.collision_obstacle);
        TF_CHECK_NEAR(a.elapsed_time, b.elapsed_time, 0.0001f);
        TF_CHECK_NEAR(a.player.y, b.player.y, 0.0001f);
        TF_CHECK_INT(a.score.current, b.score.current);
        TF_CHECK_INT(a.seed, b.seed);
        TF_CHECK_INT(a.terrain.segment_count, b.terrain.segment_count);
        for (i = 0; i < a.terrain.segment_count; ++i) {
            TF_CHECK_NEAR(a.terrain.segments[i].x, b.terrain.segments[i].x, 0.0001f);
            TF_CHECK_INT(a.terrain.segments[i].pattern, b.terrain.segments[i].pattern);
        }
    }

    /* 自动玩家连续跑 120 秒，验证地形可玩性与长局稳定性。 */
    {
        int steps = 0;
        game_init(&state, 0xBEEF1234u);
        run_autoplay(&state, SP_TARGET_FPS * 120, &steps);
        if (state.phase == GAME_PHASE_GAME_OVER) {
            printf("  autoplay died at %.2fs (step %d), score %d\n",
                   (double)state.elapsed_time, steps, state.score.current);
        }
        TF_CHECK(state.phase == GAME_PHASE_PLAYING);
        TF_CHECK(state.elapsed_time >= 119.9f);
        TF_CHECK(state.score.current > 1000);
        TF_CHECK(state.difficulty.speed > 480.0f);
        TF_CHECK_INT(state.difficulty.level, 7);
    }
}
