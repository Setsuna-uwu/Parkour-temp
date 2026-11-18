/*
 * 平台抽象层：唯一允许包含 <graphics.h>/<easyx.h> 的目录。
 * 对外全部使用 C 链接，core 与 main 不接触 EasyX 类型。
 */
#ifndef SP_PLATFORM_H
#define SP_PLATFORM_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "core/game_state.h"
#include "core/input_abstraction.h"

typedef struct Platform {
    bool initialized;
    bool quit;
    int width;
    int height;
    void *hwnd;
    double qpc_frequency;
    long long qpc_origin;
    double start_time;
} Platform;

bool platform_init(Platform *platform, const wchar_t *title, int width, int height);
void platform_shutdown(Platform *platform);
void platform_pump(Platform *platform);
void platform_poll_input(Platform *platform, Input *input);
void platform_render(Platform *platform, const GameState *state, double fps);
void platform_present(Platform *platform);
bool platform_should_quit(const Platform *platform);

void platform_time_init(Platform *platform);
double platform_now_seconds(const Platform *platform);
void platform_sleep_seconds(double seconds);

bool platform_storage_load_highscore(int *out_value);
bool platform_storage_save_highscore(int value);

void platform_crt_init(void);

#ifdef __cplusplus
}
#endif

#endif /* SP_PLATFORM_H */
