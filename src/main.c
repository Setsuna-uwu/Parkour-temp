/*
 * 雪原跑酷 - 游戏入口和固定时间步主循环。
 * 本文件是纯 C，不包含 EasyX 头文件；平台差异全部在 platform/ 内。
 */
#include <stdint.h>

#include "core/config.h"
#include "core/game.h"
#include "core/score.h"
#include "platform/platform.h"

int main(void)
{
    Platform platform;
    GameState state;
    Input input;
    int loaded_high_score = 0;
    double last_time;
    double accumulator = 0.0;
    double fps = (double)SP_TARGET_FPS;
    const double frame_interval = 1.0 / (double)SP_TARGET_FPS;
    const double fixed_step = (double)SP_FIXED_DT;

    platform_crt_init();

    if (!platform_init(&platform, L"雪原跑酷  ·  C 项目", SP_SCREEN_WIDTH, SP_SCREEN_HEIGHT)) {
        return 1;
    }

    game_init(&state, 0x5EED2026u);
    if (platform_storage_load_highscore(&loaded_high_score)) {
        score_set_high_score(&state.score, loaded_high_score);
    }

    last_time = platform_now_seconds(&platform);

    while (!state.quit_requested && !platform_should_quit(&platform)) {
        double frame_start;
        double now;
        double frame_time;
        double frame_end;
        int steps = 0;

        frame_start = platform_now_seconds(&platform);
        platform_pump(&platform);
        platform_poll_input(&platform, &input);

        now = platform_now_seconds(&platform);
        frame_time = now - last_time;
        last_time = now;
        if (frame_time < 0.0) {
            frame_time = 0.0;
        }
        if (frame_time > 0.25) {
            frame_time = 0.25;
        }
        if (frame_time > 0.0001) {
            double instant_fps = 1.0 / frame_time;
            if (instant_fps > 1000.0) {
                instant_fps = 1000.0;
            }
            fps = fps * 0.90 + instant_fps * 0.10;
        }

        accumulator += frame_time;

        {
            Input step_input = input;
            while (accumulator >= fixed_step && steps < 8) {
                game_update(&state, &step_input);
                input_clear_edges(&step_input);
                accumulator -= fixed_step;
                steps += 1;
                if (state.quit_requested) {
                    break;
                }
            }
            if (steps >= 8) {
                /* 长时间卡顿后丢弃积压，避免追帧雪崩。 */
                accumulator = 0.0;
            }
        }

        if (state.score.high_score_dirty) {
            platform_storage_save_highscore(state.score.high_score);
            state.score.high_score_dirty = false;
        }

        platform_render(&platform, &state, fps);
        platform_present(&platform);

        if (state.quit_requested) {
            break;
        }

        frame_end = platform_now_seconds(&platform);
        {
            double used = frame_end - frame_start;
            if (used < frame_interval) {
                platform_sleep_seconds(frame_interval - used);
            }
        }
    }

    platform_shutdown(&platform);
    return 0;
}
