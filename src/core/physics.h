/*
 * 玩家垂直物理：重力、跳跃、二段跳、土狼时间、跳跃缓冲。
 * 横向位移由世界滚动表达，玩家屏幕 x 保持不变。
 */
#ifndef SP_PHYSICS_H
#define SP_PHYSICS_H

#include <stdbool.h>

#include "core/input_abstraction.h"

typedef struct Player {
    float x;
    float y;
    float vy;
    bool on_ground;
    int jumps_used;
    float coyote_timer;
    float jump_buffer_timer;
    float run_phase;
} Player;

void physics_reset(Player *player, float x, float ground_y);
void physics_update(Player *player, const Input *input, float ground_y, float dt);

#endif /* SP_PHYSICS_H */
