/*
 * 雪原跑酷 - 全局常量
 * 纯 C，不依赖任何平台头文件。
 */
#ifndef SP_CONFIG_H
#define SP_CONFIG_H

#define SP_GAME_VERSION "0.1.0"
#define SP_GAME_NAME "雪原跑酷"

#ifndef SP_DEBUG_HUD
#define SP_DEBUG_HUD 1
#endif

#define SP_SCREEN_WIDTH 1280
#define SP_SCREEN_HEIGHT 720
#define SP_TARGET_FPS 60
#define SP_FIXED_DT (1.0f / 60.0f)

#define SP_GROUND_Y 612.0f
#define SP_PLAYER_START_X 260.0f
#define SP_PLAYER_W 42.0f
#define SP_PLAYER_H 56.0f

#define SP_GRAVITY 2100.0f
#define SP_JUMP_SPEED (-880.0f)
#define SP_MAX_FALL_SPEED 1500.0f
#define SP_COYOTE_TIME 0.10f
#define SP_JUMP_BUFFER_TIME 0.13f
#define SP_MAX_JUMPS 2

#define SP_TERRAIN_MAX_SEGMENTS 40
#define SP_TERRAIN_MAX_OBSTACLES 4
#define SP_TERRAIN_AHEAD 900.0f
#define SP_TERRAIN_RECYCLE_X (-180.0f)
#define SP_TERRAIN_SAFE_LEAD 1180.0f

#define SP_SCORE_PIXELS_PER_METER 45.0f
#define SP_SCORE_TIME_BONUS 6.0f
#define SP_HIGH_SCORE_LIMIT 100000000

#endif /* SP_CONFIG_H */
